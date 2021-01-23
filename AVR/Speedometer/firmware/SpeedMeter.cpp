#include <avr/io.h>
#include "ADConvert.h"
#include "SpeedMeter.h"

#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)


SpeedMeter::SpeedMeter(ADConvert* adc_,double time_stp){
	this->adc = adc_;
	this->speed_val=0;
	this->passing_flg=1;
	this->time_stp=time_stp;
}

void SpeedMeter::calculate_speed(){
	// スピードを計算
	this->speed_val = (this->wheel_len*3.14)/(this->time_val);// m/s
	// km/hに修正
	this->speed_val = this->speed_val/1000.0*60*60;

}


// 車輪の状態を読み，値を更新
double SpeedMeter::read_speed(){
	double V;
	V = this->adc->read_ADC_val();


	if( V >= 3.0){//磁石を検知
		
		if(passing_flg){//通過中
			time_val+=this->time_stp;//10ms追加

		}else{//新規通過を検知
			
			this->calculate_speed();

			passing_flg=1;// 通過中に設定
			time_val=0;
		}
		
	}else{
		passing_flg=0;//通過中を介助
		time_val+=this->time_stp;//10ms追加
	}


	return this->speed_val;
}


// 数字を７セグモジュールで表示
void SpeedMeter::disp_number(int N){

	PORTB=0b01111111;
	for (int i = 0; i <= 6; ++i){
		// ibit目取得
		if(0b0001<<i & N) CBI(PORTB, i);
	}
}

