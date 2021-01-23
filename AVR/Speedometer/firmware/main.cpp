#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "ADConvert.h"
#include "SpeedMeter.h"

#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)

#define ADC_PIN 0

// 数字を７セグモジュールで表示
void disp_number(int N){

	PORTB=0b01111111;
	for (int i = 0; i <= 6; ++i){
		// ibit目取得
		if(0b0001<<i & N) CBI(PORTB, i);
	}
}

// ADConvert* adc;
// adc = new ADConvert(ADC_PIN);
ADConvert adc(ADC_PIN,4.8);//AD変換クラス
SpeedMeter SPDM(&adc,0.01);
double speed_val;

ISR(TIMER1_OVF_vect)// Bottomに戻った時//10ms周期
{
	speed_val=SPDM.read_speed();
	disp_number((int)speed_val);

}

void init_AVR(){
	// 2桁表示モジュールへの入力用
	DDRB=0b01111111;
	PORTB=0b01111111;

	// タイマー割り込みの設定
	// タイマー０を設定．35kHzでLEDを点滅させる
	// stp = 0.125us(1分周）
	// stp = 1us(8分周）
	// stp = 8us(64分周）これ
	// stp = 32us(256分周）
	// stp = 0.128 ms(1024分周）

	TCCR1A = (1 << WGM11) | (1 << WGM10);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);

	OCR1A = 10000;//T=10ms
	OCR1B = 0;//OCR0B = 68;

	// 切断
	CBI(TCCR1A, COM1B1);
	CBI(TCCR1A, COM1B0);

	//タイマー割り込みの許可
	TIMSK1 = (1 <<  TOIE1) | (0<< OCIE1A) | (0<< OCIE1B);


	sei();//割り込み許可

}

int main(){

	init_AVR();

	double V;

	// double dT=0;//経過時間

	while(1){		
		
		// V = adc.read_ADC_val();

		// V=V*10.0;
		// disp_number(V);//速度値を正数値として，表示

		// _delay_us(50000);
		



	}


	return 0;
}




