#include <avr/io.h>
#include "ADConvert.h"


ADConvert::ADConvert(int AD_PIN,double ValMax){

	this->ValMax=ValMax;

	// 各種レジスタの設定
	int mux=0b0000;//ADC0を利用
	// ADMUX  = _BV(REFS1) |_BV(REFS0) | _BV(ADLAR) | mux; // Vref = 1.1V, left aligned 
	ADMUX  = _BV(REFS0) | _BV(ADLAR) | mux; // Vref = AVCC, left aligned 
	// ADCSRA = _BV(ADEN) | 0b111; 
	ADCSRA = _BV(ADEN) | AD_PIN; 

	ADCSRB = 0;

}

void ADConvert::start_ADC(){
	ADCSRA |= _BV(ADSC); // start conversion 
	while (ADCSRA & _BV(ADSC));//AD変換が終わるまで待つ
}

void ADConvert::update_ADC_val(){// 値を読む
	int val=0;
	double V;//[V]電圧値

	// ADCLを最初に読み，次にADCHを読む必要がある．
	// 10bitとして取得
	val = ADCL >> 6;
	val |= ADCH << 2;

	// 電圧値に変換
	V=val/1024.0*ValMax;//4.67は，Vrefの電圧値．状況によって補正が必要かも
	this->AD_val=V;

}

double ADConvert::read_ADC_val(){
	this->start_ADC();
	this->update_ADC_val();
	return this->AD_val;

}