// 時間を計測するためのライブラリ
#ifndef INCLUDE_GUARD_TimeCounter
#define INCLUDE_GUARD_TimeCounter
#include <avr/io.h>
#include "Timer0.h"

class TimeCounter:public Timer0{
public:
	int state;//0:カウントストップ，1:カウント中
	long int count;//longでないと，オーバーフローする．
	double stp;//整数だとダメ
	// コンストラクタ 
	TimeCounter(int division,int T):Timer0(division,T,0){
		// 割り込みの設定
		Timer0::set_interrupt(1);//OCR1A割り込み
		Timer0::start_interrupt();//親クラスのメゾッドはこう呼び出す

		// 割り込み周期
		// this->stp=Timer0::interrupt_period;
		this->stp=0.125*division*T;

		this->state=0;
		this->count=0;
	}

    void update_TC(){//割り込み関数内で利用
    	if(state)count++;
    }

    void start_TC(){
    	count=0;
    	state=1;
    }
    void stop_TC(){
    	state=0;
    }

    // countを秒数に変換
    double get_useconds(){
		return count*stp;
	}

	double get_mseconds(){
		return count*stp*1e-3;
	}

	double get_seconds(){
		return count*stp*1e-6;
	}
};


#endif 

