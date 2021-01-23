#ifndef INCLUDE_GUARD_Timer1
#define INCLUDE_GUARD_Timer1

#include "ABTimer.h"
#include <avr/io.h>
#include <avr/interrupt.h>


class Timer1: public ABTimer{

private:

public:
	int division;//分周
	int interrupt_period;//タイマー割り込みの周期[us]

	Timer1();
	Timer1(int division,int OCRA, int OCRB);

	void set_outputA(int mode);

	void set_outputB(int mode);

	void set_OCRA(int T){
		OCR1A = T;
	}

	void set_OCRB(int T){
		OCR1B = T;
	}

	void set_PWM_mode();


	void set_division(int division);

    void set_interrupt(int interrupt_type);//割り込みの設定
	
	void start_interrupt();

};



#endif 