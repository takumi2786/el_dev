/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */


// 赤外線通信
// 関数でまとめてみた
// ちゃんと動くか要確認

// これめっちゃ使える．
#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)
// #define SW() (PINB & 0x0f) 
#define F_CPU 8000000UL//内部クロックの周波数

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// 数字を７セグモジュールで表示
void disp_number(int N){
	PORTB=0b01111111;
	for (int i = 0; i <= 6; ++i){
		// ibit目取得
		if(0b0001<<i & N) CBI(PORTB, i);
	}

}

//0b1110
//0b0001

int main(){

	DDRB=0b01111111;
	PORTB=0b01111111;

	// ADC関係レジスタの設定
	// CBI(PRR,0);//ADCを有効化
	// DIDR0  = 0b00111111; // disable digital input component 

	int mux=0b0000;//ADC0を利用
	// ADMUX  = _BV(REFS1) |_BV(REFS0) | _BV(ADLAR) | mux; // Vref = 1.1V, left aligned 
	ADMUX  = _BV(REFS0) | _BV(ADLAR) | mux; // Vref = AVCC, left aligned 

	// ADMUX=0b00100000;
	


	
	ADCSRA = _BV(ADEN) | _BV(ADSC) | 0b111; // start conversion 
	ADCSRB = 0;

	while(1){
		int val=0;
		
		ADCSRA |= _BV(ADSC); 
		while (ADCSRA & _BV(ADSC));//AD変換が終わるまで待つ

		// 値を読む
		// ADCLを最初に読み，次にADCHを読む必要がある．
		// 10bitとして取得
		val = ADCL >> 6;
  		val |= ADCH << 2;


  		// 8bitの分解能に設定
  		int N=val;

  		// Vの値に変換
  		double Vin;//[*0.1V]
  		Vin=N/1024.0*46.7;


		// val = ADCH;//8bitの値として取得
		// 分解能が少し下がるが，簡単
  		// int N=val/255*99;
 		
 		// int N=val;

		disp_number((int)Vin);
		_delay_us(100000);
	}


	return 0;
}






// #define SwitchPin1 3
// #define SwitchPin2 6
// #define OutPin 5
// #define DataNum 18

// // PWMの値書き換え許可フラグ
// volatile bool isPWMchangeable = false;
// // ISR(TIMER1_OVF_vect)// タイマー１がBottomに戻った時の割り込み処理
// // ISR(TIMER1_CAPT_vect)
// ISR(TIMER1_COMPB_vect)
// {

// 	CBI(TCCR0A, COM0B1);
// 	CBI(TCCR0A, COM0B0);

// 	//切り替えを許可
// 	isPWMchangeable=true;

// }


// ISR(TIMER1_OVF_vect)// Bottomに戻った時
// {

// 	// ３５kHzのPWMを出力
// 	SBI(TCCR0A, COM0B1);
// 	CBI(TCCR0A, COM0B0);

// }


// int on_data[DataNum]={// ONボタン
// 	0x23,
// 	0xCB,
// 	0x26,
// 	0x1,
// 	0x0,
// 	0x20,
// 	0x8,
// 	0x7,
// 	0x30,
// 	0x49,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0xBD
// };

// int off_data[DataNum]={// ONボタン
// 	0x23,
// 	0xCB,
// 	0x26,
// 	0x1,
// 	0x0,
// 	0x0,
// 	0x8,
// 	0x7,
// 	0x30,
// 	0x49,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0x0,
// 	0x9D
// };


// // int data_array[DataNum]={// ONボタン
// // 	0x23
// // };

// int read_switch(){
// 	int state=0;
//     bool state_c;
// 	//switch の状態を取得
// 	state_c =  !(( PINB >> SwitchPin1) & 0b01);
// 	if(state_c)state=1;
//     state_c =  !(( PIND >> SwitchPin2) & 0b01);
//     if(state_c)state=2;

// 	return state;
// }

// void send_leader(){
	

// 	// PWMの設定を更新可能になるまで待つ
// 	while(1){
//       if(isPWMchangeable==true)break;
// 	}
// 	// 出力設定
// 	// DDRD = (1 << DDD5);//PD5
// 	SBI(DDRD,DDD5);

// 	// リーダー
// 	OCR1A = 600;
// 	OCR1B = 400;

// 	//切り替えを禁止
// 	isPWMchangeable=false;

	

// 	// _delay_us(4400);
// }
// void send_1bit(bool bit){
// 	// PWMの設定を更新可能になるまで待つ
// 	while(1){
//       if(isPWMchangeable==true)break;
// 	}
// 	// PWMの設定を更新
// 	if(bit==true){
// 		// on
// 		OCR1A = 200;
// 		OCR1B = 50;
// 	}else{
// 		// off
// 		OCR1A = 100;
// 		OCR1B = 50;
// 	}


// 	//切り替えを禁止
// 	isPWMchangeable=false;

// }

// void send_data(int state, int* data_array){ // 8bit*DataNum分のデータを送信
// 	int data;
// 	bool bit_cr=false;

// 	for (int i = 0; i < DataNum; ++i){
// 		data=data_array[i];
// 		for (int j = 0; j < 8; ++j){ //8bitを順番に送る
// 			bit_cr = (data>>j)&1;//j個目のbitを取得
// 			// 送信
// 			send_1bit(bit_cr);
// 		}
// 	}
// }


// void send_last_pulse(){
// 	// PWMの設定を更新可能になるまで待つ
// 	while(1){
//       if(isPWMchangeable==true)break;
// 	}
// 	// PWMの設定を更新
// 	OCR1A = 212;
// 	OCR1B = 50;
// 	//切り替えを禁止
// 	isPWMchangeable=false;


// 	// _delay_us(1600);
// }





// void send(int state, int* data_array){
	

// 	CBI(DDRD,DDD5);//PD5//切断
// 	sei();//割り込み許可

// 	for (int k = 0; k < 2; ++k){// 二回繰り返す
// 		// 第k波
// 		send_leader();// リーダーコード
		
// 		send_data(state, data_array);// dataを送信

// 		send_last_pulse();// ラストのパルス
// 	}

// 	// 最後の波形がON->OFFになるまで待つ
// 	while(1){
//       if(isPWMchangeable==true)break;
// 	}

// 	// 停止
// 	cli();
// 	CBI(TCCR0A, COM0B1);
// 	CBI(TCCR0A, COM0B0);
	
// }





// // bool state=false;//センサの状態

// int main(){

// 	//スイッチピンの設定
// 	CBI(DDRB, SwitchPin1);//入力状態
//     SBI(PORTB, SwitchPin1);//HIGHにする．

//     CBI(DDRD, SwitchPin2);//入力状態
//     SBI(PORTD, SwitchPin2);//HIGHにする．


// 	// 16bit Timer
// 	// 出力ピン PB5
// 	SBI(DDRD, DDD5);

// 	// タイマー０の設定．35kHzでLEDを点滅させる
// 	// stp = 0.125us(1分周）
// 	TCCR0A = (1 << WGM01) | (1 << WGM00);
// 	TCCR0B = (1 << WGM02) | (0 << CS02) | (0 << CS01) | (1 << CS00);

// 	// 周波数＝38kHz になるように設定．
// 	OCR0A = 212;
// 	OCR0B = 68;//OCR0B = 68;

// 	// 切断
// 	CBI(TCCR0A, COM0B1);
// 	CBI(TCCR0A, COM0B0);


// 	// タイマー１の設定．35kHzの点灯をオンオフする．
// 	// 64分周
// 	// stp = 8us
// 	TCCR1A = (1 << WGM11) | (1 << WGM10);
// 	TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);

// 	OCR1A = 0;
// 	OCR1B = 0;

// 	// 割り込み設定
// 	TIMSK = 0b10100000;//タイマー割り込みの許可

// 	// 切断
// 	CBI(TCCR1A, COM1B1);
// 	CBI(TCCR1A, COM1B0);

// 	int state = 0;//センサの状態

// 	// sei();//割り込み許可


// 	// データベクトル保存用ポインタ
// 	int* data_array;// 配列格納用ポインタ
// 	while(1){
// 		state=read_switch();

// 		if(state){
// 			// データを獲得
// 			if(state==1)data_array=on_data;
// 			else if(state==2)data_array=off_data;


// 	    	send(state,data_array);
// 	    	// PORTB ^= 0b00010000;
// 	    	_delay_us(500000);
// 	  	}

// 	}
	



// 	return 0;
// }

/*
便利なコードをメモしておくよ
・指定したbitをONする
#define SBI(reg,bit) reg |= (1 << bit)
・指定したbitをOFFする
#define CBI(reg,bit) reg &= ~(1 << bit)


ビットの反転 ＞＞ LEDのオンオフなど
PORTB ^= 0b00010000;




*/



