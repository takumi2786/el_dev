// サーボモータと連動させてみた

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "./main.h"
// 自作ライブラリ
#include "DGRead.h"
#include "IRCommunication.h"
#include "TimeCounter.h"//Timer0を利用
#include "Timer1.h"
#include "Servo.h"

int division = 8;//分周
int T = 50;　　　 //周期

TimeCounter TC(division,T);
DGRead DGR_C0(C,0);//デジタルリード
IRCommunication IRC(&TC,&DGR_C0);

Timer1 timer1;//タイマー１を利用
Servo servo(&timer1);// サーボモータ-

ISR(TIMER0_COMPA_vect){
	IRC.update_time_counter();
}

int main(){
	// 初期化
	init_display();
	int data[DataNum];//読み込んだ値を格納する配列
	// TC.start_TC();//タイマースタート
	while(1){		
		disp_number(0);
		IRC.read_data(&data[0]);
		disp_number(1);

		bool NG_flg;
		NG_flg = IRC.judge_data(&data[0]);

		if(!NG_flg){
			disp_number(31);
			servo.change_deg(90);
		}else{
			disp_number(13);
			servo.change_deg(0);
		}
		
		_delay_ms(500);
	}
	
	return 0;
}










