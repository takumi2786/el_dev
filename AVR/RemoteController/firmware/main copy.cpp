// 赤外線通信
// 温度操作に対応
// ATMEGA用

// ビット操作のマクロ．
#define SBI(reg,bit) reg |= (1 << bit)  // 選択したbitを1にする．
#define CBI(reg,bit) reg &= ~(1 << bit) // 選択したbitを0にする．
#define CHBI(req,bit) (req >> bit) & 0b01 //選択したbitが1ならTrueを返す．

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SwitchPin1 0 //PD0
#define SwitchPin2 3 //PD3
#define SwitchPin3 4 //PB4
#define SwitchPin4 6 //PD6

#define OutPin  5 //PD5
#define DataNum 4
#define DataLen 1

// PWMの値書き換え許可フラグ
volatile bool PwmState = false;


// 設定など
// 動作周波数: 8000000Hz
//   1クロック＝0.125us
// タイマー1：
// PWMキャリア周期を制御する．
// 設定：
// WGM1=1111 (高速PWM) 
//  0->OCR1A
// CS1=011   (64分周)
//  0.125*64*10^-7=8 us 

// タイマー０:
// PWMのサブキャリア周波数を制御する．
// 設定:
// WGM0 = 111 (高速PWM) 
//  0->OCR0A
// CS0 = 001   (1分周)
//  0.125*1*10^-7=0.125 us 
// COM0B1:
//   キャリアがHIGH状態の時：COM0B1=10 比較一致でLOW，BottomでHIGHをOC0B＝PD5に出力
//   キャリアがLOW状態の時： COM0B1=00 切断


// PwmState:
// PWMがHIGH状態の間false，LOW状態になるとfalse



// リモコン送信データ
int data_array[DataNum]={// ONボタン 冷房
	0,
	1,
	0,
	0
};

// タイマー１割り込みでPWMキャリアを制御
ISR(TIMER1_COMPB_vect)// TCNT1とOCR1Bが一致したとき．
{
	// OC0B切断
	CBI(TCCR0A, COM0B1);
	CBI(TCCR0A, COM0B0);

	PwmState = false;
}

ISR(TIMER1_OVF_vect)// Bottomに戻った時
{
	// OC0B出力
	SBI(TCCR0A, COM0B1);
	CBI(TCCR0A, COM0B0);

	// SBI(PORTD,OutPin);
	PwmState = true;
}


// レジスタの設定など
void initialize();

// スイッチを読む
int read_switch();

// 1bit分送信する．
void send_1bit(bool bit);

// リーダーコード
void send_leader();

// 最後に1つbitを送信
void send_last_pulse();

// 8bit*DataNum分のデータを送信
void send_data(int* data_array);

// PWM周期を変更
void change_pulse_period(int OCR1A_, int OCR1B_ );

int main(){
	initialize();

	// PwmState = true;
	int state;

	while(1){

		state=read_switch();
		if (state==1){
			sei();
			TCNT1=0; // カウントの初期化 これによって波形が安定する．

			change_pulse_period(100,50);
			change_pulse_period(100,50);
			change_pulse_period(100,50);
			change_pulse_period(100,50);
			change_pulse_period(100,50);


		
			cli();//割り込みの停止
			// 切断
			CBI(TCCR0A, COM0B1);
			CBI(TCCR0A, COM0B0);
			_delay_us(500000);

		}
	}
	return 0;
}

void initialize(){
	//スイッチピンの設定
	CBI(DDRD, SwitchPin1);//入力状態
    SBI(PORTD, SwitchPin1);//HIGHにする．
    CBI(DDRD, SwitchPin2);//入力状態
    SBI(PORTD, SwitchPin2);//HIGHにする．
    CBI(DDRB, SwitchPin3);//入力状態
    SBI(PORTB, SwitchPin3);//HIGHにする．
    CBI(DDRD, SwitchPin4);//入力状態
    SBI(PORTD, SwitchPin4);//HIGHにする．

	// 出力ピン OC0B=PB5
	SBI(DDRD, OutPin);
	// タイマー０の設定．
	// 35kHzでLEDを点滅させる
	// 1分周: stp = 0.125us 212*0.125us = 26.5[us] = 38kHz
	
	TCCR0A = (1 << WGM01) | (1 << WGM00);
	TCCR0B = (1 << WGM02) | (0 << CS02) | (0 << CS01) | (1 << CS00);

	// サブキャリア周波数を38khzに設定
	OCR0A = 212;
	OCR0B = 100;

	// 切断
	CBI(TCCR0A, COM0B1);
	CBI(TCCR0A, COM0B0);

	// タイマー１の設定．PWMのキャリアを制御
	// 64分周: stp = 8us 50*8=400us 
	TCCR1A = (1 << WGM11) | (1 << WGM10);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);

	// 一旦0で初期化
	// OCR1A = 0;
	// OCR1B = 0;

	// 割り込み設定
	TIMSK1 = (1 <<  TOIE1) | (1<< OCIE1B);//タイマー割り込みの許可

	// 切断 出力はしない
	CBI(TCCR1A, COM1B1);
	CBI(TCCR1A, COM1B0);
}

// スイッチを読む
int read_switch(){
	int state=0;
    // bool state_c;
	//switch の状態を取得
	if     ( !(( PIND >> SwitchPin1) & 0b01) ) return 1;
    else if( !(( PIND >> SwitchPin2) & 0b01) ) return 2;
    else if( !(( PINB >> SwitchPin3) & 0b01) ) return 3;
    else if( !(( PIND >> SwitchPin4) & 0b01) ) return 4;

	return state;
}

// PWM周期を変更
void change_pulse_period(int OCR1A_, int OCR1B_ ){
	// OCR1A_: PWMの周期
	// OCR1B_: Onの時間
	// while( PwmState==true ); // PWMが出力状態の間待つ．
	OCR1A = OCR1A_;
	OCR1B = OCR1B_;
	// PwmState = true;
	// _delay_us(60*8);
}

void send_1bit(bool bit){
	if(bit==true){
		// on
		change_pulse_period(200,50);
	}else{
		// off
		change_pulse_period(100,50);
	}
}

void send_leader(){
	change_pulse_period(200,40);
}

void send_last_pulse(){
	change_pulse_period(200,50);
}

// 8bit*DataNum分のデータを送信
void send_data(int* data_array){ 
	int data;
	bool bit_cr=false;

	for (int i = 0; i < DataNum; ++i){
		data=data_array[i];
		for (int j = 0; j < DataLen; ++j){ //8bitを順番に送る
			bit_cr = (data>>j)&1;//j個目のbitを取得
			// 送信
			send_1bit(bit_cr);
		}
	}

}



/*
ISR(TIMER1_COMPB_vect)// TCNT1とOCR1Bが一致したとき．
{
	CBI(TCCR0A, COM0B1);
	CBI(TCCR0A, COM0B0);
	//切り替えを許可
	isPWMchangeable=true;
}

ISR(TIMER1_OVF_vect)// Bottomに戻った時
{
	// ３５kHzのPWMを出力
	SBI(TCCR0A, COM0B1);
	CBI(TCCR0A, COM0B0);
}


// 24度
int on_data_cool[DataNum]={// ONボタン 冷房
	0x23,
	0xCB,
	0x26,
	0x1,
	0x0,
	0x20,
	0x18,
	0x8,
	0x36,
	0x49,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xD4
};

// Offボタン
int off_data[DataNum]={
	0x23,
	0xCB,
	0x26,
	0x1,
	0x0,
	0x0,
	0x18,
	0x8,
	0x36,
	0x49,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xB4
};

int read_switch(){
	int state=0;
    // bool state_c;
	//switch の状態を取得
	if     ( !(( PIND >> SwitchPin1) & 0b01) ) return 1;
    else if( !(( PIND >> SwitchPin2) & 0b01) ) return 2;
    else if( !(( PINB >> SwitchPin3) & 0b01) ) return 3;
    else if( !(( PIND >> SwitchPin4) & 0b01) ) return 4;

	return state;
}

void send_leader(){
	// PWMの設定を更新可能になるまで待つ
	while(1){
      if(isPWMchangeable==true)break;
	}
	// 出力設定
	SBI(DDRD,OutPin);

	// リーダー
	OCR1A = 600;
	OCR1B = 400;

	//切り替えを禁止
	isPWMchangeable=false;
	// _delay_us(4400);
}

void send_1bit(bool bit){
	// PWMの設定を更新可能になるまで待つ
	while(1){
      if(isPWMchangeable==true)break;
	}
	// PWMの設定を更新
	if(bit==true){
		// on
		OCR1A = 200;
		OCR1B = 50;
	}else{
		// off
		OCR1A = 100;
		OCR1B = 50;
	}

	//切り替えを禁止
	isPWMchangeable=false;
}

void send_data(int state, int* data_array){ // 8bit*DataNum分のデータを送信
	int data;
	bool bit_cr=false;

	for (int i = 0; i < DataNum; ++i){
		data=data_array[i];
		for (int j = 0; j < 8; ++j){ //8bitを順番に送る
			bit_cr = (data>>j)&1;//j個目のbitを取得
			// 送信
			send_1bit(bit_cr);
		}
	}
}

void send_last_pulse(){
	// PWMの設定を更新可能になるまで待つ
	while(1){
      if(isPWMchangeable==true)break;
	}
	// PWMの設定を更新
	OCR1A = 212;
	OCR1B = 50;
	//切り替えを禁止
	isPWMchangeable=false;
}

void send(int state, int* data_array){

	CBI(DDRD,DDD5);//PD5//切断
	sei();//割り込み許可

	for (int k = 0; k < 2; ++k){// 二回繰り返す
		// 第k波
		send_leader();// リーダーコード
		
		send_data(state, data_array);// dataを送信

		send_last_pulse();// ラストのパルス
	}

	// 最後の波形がON->OFFになるまで待つ
	while(1){
      if(isPWMchangeable==true) break;
	}

	// 停止
	cli();
	CBI(TCCR0A, COM0B1);
	CBI(TCCR0A, COM0B0);
}

void Chage_EC_State(int state){
	// Switch ON
	if(state==1)send(state,on_data_cool);
	else if(state==2){
		// 温度上昇
		// 最大値：206
		if( on_data_cool[17] < 220){
			on_data_cool[DataNum-1]+=1;
			on_data_cool[7]+=1;
		}
		send(state,on_data_cool);
	}
	else if(state==3){
		// 温度低下
		// 最小値：206
		if( on_data_cool[17] > 206){
			on_data_cool[7]-=1;
			on_data_cool[17]-=1;
		}
		send(state,on_data_cool);
	}
	else if(state==4)send(state,off_data);//Switch OFF

}

int main(){
	//スイッチピンの設定
	CBI(DDRD, SwitchPin1);//入力状態
    SBI(PORTD, SwitchPin1);//HIGHにする．

    CBI(DDRD, SwitchPin2);//入力状態
    SBI(PORTD, SwitchPin2);//HIGHにする．

    CBI(DDRB, SwitchPin3);//入力状態
    SBI(PORTB, SwitchPin3);//HIGHにする．

    CBI(DDRD, SwitchPin4);//入力状態
    SBI(PORTD, SwitchPin4);//HIGHにする．

	// 出力ピン OC0B=PB5
	SBI(DDRD, OutPin);

	// タイマー０の設定．35kHzでLEDを点滅させる
	// stp = 0.125us(1分周）
	TCCR0A = (1 << WGM01) | (1 << WGM00);
	TCCR0B = (1 << WGM02) | (0 << CS02) | (0 << CS01) | (1 << CS00);

	// 周波数＝38kHz になるように設定．
	OCR0A = 212;
	OCR0B = 68;//OCR0B = 68;

	// 切断
	CBI(TCCR0A, COM0B1);
	CBI(TCCR0A, COM0B0);

	// タイマー１の設定．35kHzの点灯をオンオフする．
	// 64分周
	// stp = 8us
	TCCR1A = (1 << WGM11) | (1 << WGM10);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);

	OCR1A = 0;
	OCR1B = 0;

	// 割り込み設定
	// TIMSK = 0b10100000;//タイマー割り込みの許可
	//タイマー割り込みの許可
	TIMSK1 = (1 <<  TOIE1) | (1<< OCIE1B);

	// 切断
	CBI(TCCR1A, COM1B1);
	CBI(TCCR1A, COM1B0);

	int state;//センサの状態
	// データベクトル保存用ポインタ
	int* data_array;// 配列格納用ポインタ
	while(1){
		state = 0;
		state=read_switch();
		// state = 1;
		if(state){
			// データを獲得
	  		Chage_EC_State(state);
	    	_delay_us(500000);
	  	}
	}
	
	return 0;
}



*/