#ifndef INCLUDE_GUARD_ABTimer
#define INCLUDE_GUARD_ABTimer
// これめっちゃ使える．
#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)


#include <avr/io.h>


// タイマーの抽象クラス
class ABTimer{

private:

public:
	int division;//分周
	int interrupt_period;//タイマー割り込みの周期[us]

	void initialise(int division,int OCRA, int OCRB){//>>これをコンストラクタに入れられない？：https://qiita.com/eierapfel/items/ad607679a9c80e9516e0
	    // ＞要確認．おそらく，コンストラクタ は，純粋仮想関数や仮想関数を呼べない．
		set_division(division);
    
		// 高速PWM
		set_PWM_mode();
		
		// カウンタ値の設定
		set_OCRA(OCRA);
		set_OCRB(OCRB);
	}

	virtual void set_PWM_mode()=0;
	virtual void set_outputA(int mode)=0;
	virtual void set_outputB(int mode)=0;
	virtual void set_OCRA(int T)=0;
	virtual void set_OCRB(int T)=0;
	virtual void set_division(int division)=0;
    virtual void set_interrupt(int interrupt_type)=0;//割り込みの設定
	virtual void start_interrupt()=0;
};



#endif 