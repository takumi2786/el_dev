#ifndef INCLUDE_GUARD_Timer
#define INCLUDE_GUARD_Timer
//16ビットタイマー
// 高速PWM専用

class Timer{

private:

public:
	int division;//分周
	int interrupt_period;//タイマー割り込みの周期[us]

	Timer();
	Timer(int division,int OCR1A_, int OCR1B_);

	void set_outputA(int mode);

	void set_outputB(int mode);

	void set_OCR1A(int T){
		OCR1A = T;
	}

	void set_OCR1B(int T){
		OCR1B = T;
	}


	void set_division(int division);

    void set_interrupt(int interrupt_type);//割り込みの設定
	
	void start_interrupt();

};



#endif 