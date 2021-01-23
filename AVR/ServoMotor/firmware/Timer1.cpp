
#include "Timer1.h"// これめっちゃ使える．
#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)

// 改善点
// ・割り込み設定をクラス内部で行いたい
// ・8bitタイマーにも対応したい

Timer1::Timer1():ABTimer(){
    set_PWM_mode();
    
}

Timer1::Timer1(int division,int OCRA, int OCRB):ABTimer(){
    initialise(division,OCRA,OCRB);
    // division: 分周(1,8,64,256,1024)
    // 分周の設定
    // set_division(division);
    
    // // 高速PWM
    // set_PWM_mode();
    
    // // カウンタ値の設定
    // set_OCRA(OCRA);
    // set_OCRB(OCRB);
}

void Timer1::set_PWM_mode(){
    // 高速PWM
    TCCR1A |= (1 << WGM11) | (1 << WGM10);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
}


void Timer1::set_outputA(int mode){
    //0:標準ポート動作 (OC1x切断)
    //1:比較一致でOC1Aピン トグル(交互)出力、OC1Bは標準ポート動作(OC1B切断)
    //2:比較一致でLow、BOTTOMでHighをOC1Aピンへ出力 (非反転動作)
    //3:比較一致でHigh、BOTTOMでLowをOC1Aピンへ出力 (反転動作)
    
    switch(mode){
        case 0:
            TCCR1A |= (0 << COM1A1) | (0 << COM1A0);
            break;
        case 1:
            TCCR1A |= (0 << COM1A1) | (1 << COM1A0);
            break;
        case 2:
            TCCR1A |= (1 << COM1A1) | (0 << COM1A0);
            break;
        case 3:
            TCCR1A |= (1 << COM1A1) | (1  << COM1A0);
            break;
    }
}

void Timer1::set_outputB(int mode){
    //0:標準ポート動作 (OC1x切断)
    //1:比較一致でOC1Aピン トグル(交互)出力、OC1Bは標準ポート動作(OC1B切断)
    //2:比較一致でLow、BOTTOMでHighをOC1Bピンへ出力 (非反転動作)
    //3:比較一致でHigh、BOTTOMでLowをOC1Bピンへ出力 (反転動作)
    
    switch(mode){
        case 0:
            CBI(DDRB, DDB2);
            TCCR1A |= (0 << COM1B1) | (0 << COM1B0);
            break;
        case 1:
            SBI(DDRB, DDB2);
            TCCR1A |= (0 << COM1B1) | (1 << COM1B0);
            break;
        case 2:
            SBI(DDRB, DDB2);
            TCCR1A |= (1 << COM1B1) | (0 << COM1B0);
            break;
        case 3:
            SBI(DDRB, DDB2);
            TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
            break;
    }

}


void Timer1::set_division(int division){
    // 分周の設定
    this->division=division;
    switch(division){
        case 1: // stp = 0.125us(1分周）
            TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
        break;

        case 8: // stp = 1us(8分周）
            TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
        break;

        case 64: // stp = 8us(64分周）
            TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
        break;

        case 256:// stp = 32us(256分周）
            TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
        break;

        case 1024: // stp = 0.128 ms(1024分周）
            TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
        break;

        default://機能停止
            TCCR1B |= (0 << CS12) | (0 << CS11) | (0 << CS10);
        break;
    }
    
}

// タイマー割り込みの有効化
void Timer1::set_interrupt(int interrupt_type){
    // interrupt  (0:TOIE1, 1:OCIE1A, 2: OCIE1B
    //タイマー割り込みの許可
    switch (interrupt_type){
    case 0:
        this->interrupt_period=0.125*division*OCR1A;//高速PWM以外だとTOP値が変わるので注意
        TIMSK1 |= (0<< TOIE1);//TOIE1割り込みを利用>>TIMER1_OVF_vect
        break;
    case 1:
        this->interrupt_period=0.125*division*OCR1A;
        TIMSK1 |= (1<< OCIE1A);//OCIE1A割り込みを利用>>TIMER1_COMPA_vect
        break;
    case 2:
        this->interrupt_period=0.125*division*OCR1B;
        TIMSK1 |= (1<< OCIE1B);//OCIE1B割り込みを利用>>TIMER1_COMPB_vect
        break;
    
    default:
        break;
    }
}

void Timer1::start_interrupt(){
    sei();
}