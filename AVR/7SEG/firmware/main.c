

// 7SEG LED ATMEGA328puß用
// 二進数4桁で入力を受け取り，7seg LEDに出力する．

// これめっちゃ使える．
#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// 入力ピン
#define InputPin0 0 //PD0
#define InputPin1 1 //PD1
#define InputPin2 2 //PD2
#define InputPin3 3 //PD3
#define InputPin4 4 //PD4
#define InputPin5 5 //PD5
#define InputPin6 6 //PD6 
// #define InputPin7 7 //PD6 

// 7seg出力
#define OutPinA 0 //PB0
#define OutPinB 1 //PB1
#define OutPinC 2 //PB2
#define OutPinD 3 //PB3
#define OutPinE 4 //PB4
#define OutPinF 5 //PB5
#define OutPinG 6 //PB6

// 桁表示フラグ 出力
#define dig_out0 0 //PC0
#define dig_out1 1 //PC1

int get_number(){
	char N=0;

	// // int var=0;
	// // 一桁目取得
	// N = N | ( PIND & (0b01<<0));
	// // 二桁目取得
	// N = N | ( PIND & (0b01<<1));
	// bool bit;

	char temp = PIND;

	// bit反転
	temp = ~temp;
	CBI(temp,7);
	// 8bit目を強制的に0にする
	N=temp;

	return (int)N;
}

void disp_dig(int N,int dig){
	// digitの選択
	// 一旦全部消す
	
	CBI(PORTC, dig_out0);
	CBI(PORTC, dig_out1);
	PORTB=0;

	// 桁を表示
	switch(dig){
		case 0:
			SBI(PORTC, dig_out0);
			CBI(PORTC, dig_out1);
		break;
		case 1:
			CBI(PORTC, dig_out0);
			SBI(PORTC, dig_out1);
		break;
	}

	// 数字を出力
	switch (N){
	  case 0:
	    SBI(PORTB, OutPinA);
		SBI(PORTB, OutPinB);
		SBI(PORTB, OutPinC);
		SBI(PORTB, OutPinD);
		SBI(PORTB, OutPinE);
		SBI(PORTB, OutPinF);
		CBI(PORTB, OutPinG);
	  break;
	  case 1:
	    CBI(PORTB, OutPinA);
		SBI(PORTB, OutPinB);
		SBI(PORTB, OutPinC);
		CBI(PORTB, OutPinD);
		CBI(PORTB, OutPinE);
		CBI(PORTB, OutPinF);
		CBI(PORTB, OutPinG);
	  break;
	  case 2:
	    SBI(PORTB, OutPinA);
		SBI(PORTB, OutPinB);
		CBI(PORTB, OutPinC);
		SBI(PORTB, OutPinD);
		SBI(PORTB, OutPinE);
		CBI(PORTB, OutPinF);
		SBI(PORTB, OutPinG);

		// PORTB=0b1011011;
	  break;
	  case 3:
	    SBI(PORTB, OutPinA);
		SBI(PORTB, OutPinB);
		SBI(PORTB, OutPinC);
		SBI(PORTB, OutPinD);
		CBI(PORTB, OutPinE);
		CBI(PORTB, OutPinF);
		SBI(PORTB, OutPinG);
	  break;
	  case 4:
	    CBI(PORTB, OutPinA);
		SBI(PORTB, OutPinB);
		SBI(PORTB, OutPinC);
		CBI(PORTB, OutPinD);
		CBI(PORTB, OutPinE);
		SBI(PORTB, OutPinF);
		SBI(PORTB, OutPinG);
	  break;
	  case 5:
	    SBI(PORTB, OutPinA);
		CBI(PORTB, OutPinB);
		SBI(PORTB, OutPinC);
		SBI(PORTB, OutPinD);
		CBI(PORTB, OutPinE);
		SBI(PORTB, OutPinF);
		SBI(PORTB, OutPinG);
	  break;
	  case 6:
	    SBI(PORTB, OutPinA);
		CBI(PORTB, OutPinB);
		SBI(PORTB, OutPinC);
		SBI(PORTB, OutPinD);
		SBI(PORTB, OutPinE);
		SBI(PORTB, OutPinF);
		SBI(PORTB, OutPinG);
	  break;
	  case 7:
	    SBI(PORTB, OutPinA);
		SBI(PORTB, OutPinB);
		SBI(PORTB, OutPinC);
		CBI(PORTB, OutPinD);
		CBI(PORTB, OutPinE);
		SBI(PORTB, OutPinF);
		CBI(PORTB, OutPinG);

		// PORTB=0b0100111;

		break;
	  case 8:
	    SBI(PORTB, OutPinA);
		SBI(PORTB, OutPinB);
		SBI(PORTB, OutPinC);
		SBI(PORTB, OutPinD);
		SBI(PORTB, OutPinE);
		SBI(PORTB, OutPinF);
		SBI(PORTB, OutPinG);
		break;
	  case 9:
	    SBI(PORTB, OutPinA);
		SBI(PORTB, OutPinB);
		SBI(PORTB, OutPinC);
		SBI(PORTB, OutPinD);
		CBI(PORTB, OutPinE);
		SBI(PORTB, OutPinF);
		SBI(PORTB, OutPinG);
		break;
	}
	
}


void disp_numbers(int N){
	int N_1, N_10;
	// 1の位を取得
	N_1 = N % 10;

	// 10の位を取得
	N=N/10;
	N_10 = N % 10;

	disp_dig(N_1,0);
	disp_dig(N_10,1);

}

void init(){
	// PIN設定
	// 入力ピン
	CBI(DDRD, InputPin0);
	CBI(DDRD, InputPin1);
	CBI(DDRD, InputPin2);
	CBI(DDRD, InputPin3);
	CBI(DDRD, InputPin4);
	CBI(DDRD, InputPin5);
	CBI(DDRD, InputPin6);
	// CBI(DDRD, InputPin7);

	// プルアップ
	SBI(PORTD, InputPin0);
	SBI(PORTD, InputPin1);
	SBI(PORTD, InputPin2);
	SBI(PORTD, InputPin3);
	SBI(PORTD, InputPin4);
	SBI(PORTD, InputPin5);
	SBI(PORTD, InputPin6);
	// SBI(PORTD, InputPin7);

	// 一桁目
	SBI(DDRB, OutPinA);
	SBI(DDRB, OutPinB);
	SBI(DDRB, OutPinC);
	SBI(DDRB, OutPinD);
	SBI(DDRB, OutPinE);
	SBI(DDRB, OutPinF);
	SBI(DDRB, OutPinG);

	// 出力する桁を選択するピン
	SBI(DDRC, dig_out0);

	SBI(DDRC, dig_out1);
	
}

int main(){
	init();
	int N=0;
	while(1){
		// 入力値を取得
		N = get_number();
		// 数字を出力
		disp_numbers(N);
		// for (int i = 0; i < 100; ++i){
			
			// _delay_ms(1000);
		// }
		// disp_number(88);
	}
	
	
	// SBI(PORTB, OutPinA);
	// SBI(PORTB, OutPinB);
	// SBI(PORTB, OutPinC);
	// SBI(PORTB, OutPinD);
	// SBI(PORTB, OutPinE);
	// SBI(PORTB, OutPinF);
	// SBI(PORTB, OutPinG);


	return 0;
}


