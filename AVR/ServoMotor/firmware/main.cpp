/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "ADConvert.h"
#include "ABTimer.h"
#include "Timer1.h"
#include "Servo.h"
#include "main.h"


#define ADC_PIN 0

// クラス変数のアドレスを確保
// ADConvert* adc;//AD変換クラス
// Servo* servo;// サーボモータ
// Timer16* timer16;// 16bitタイマ

// 静的に呼んだ方が安全っぽい

Timer1 timer1;//タイマー１を利用

Servo servo(&timer1);// サーボモータ-
double Val_Max = 91;

ADConvert adc(ADC_PIN,Val_Max);

void init_AVR(){
	// 2桁表示モジュールへの入力用
	DDRD=0b01111111;
	PORTD=0b01111111;
}


int main(){

	init_AVR();

	double adc_val;
	int angle=0;

	while(1){
		adc_val=adc.read_ADC_val();
		disp_number(adc_val);
		angle = adc_val*2;

		servo.change_deg(angle);

		// servo.change_deg(0);
		// _delay_ms(1000);
		// servo.change_deg(180);
		// _delay_ms(1000);
	}

	return 0;
}



