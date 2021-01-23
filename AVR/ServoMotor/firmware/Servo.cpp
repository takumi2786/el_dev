#include <avr/io.h>
#include "ABTimer.h"
#include "Timer1.h"
#include "Servo.h"

// コンストラクタ 
Servo::Servo(ABTimer* timer){
    // 基準パルス(us)
    this->T0=500;
    this->T180=2400;

    // タイマーの初期設定
    int division=8;
	int T_base = 20000;//20ms
	int T_on =  T180;//2.4ms(0deg)

    // タイマーのパラメータ設定
    this->timer=timer;
    timer->set_division(division);
    timer->set_OCRA(T_base);
    timer->set_OCRB(T_on);

    // 出力設定
    timer->set_outputB(2);
}

void Servo::change_deg(int deg){//角度に対応するパルス幅に変更 > OCR1Bを変える
    int T_ = calculate_T_from_deg(deg);
    timer->set_OCRB(T_);
}



int Servo::calculate_T_from_deg(double deg){
    return ( (T180-T0)/180 )*deg+T0;
}
