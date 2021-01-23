#include "Timer0.h"// これめっちゃ使える．
#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)

// 改善点
// ・割り込み設定をクラス内部で行いたい
// ・8bitタイマーにも対応したい


Timer0::Timer0(int division,int OCRA, int OCRB){
    initialise(division,OCRA,OCRB);//>>これをコンストラクタに入れられない？：https://qiita.com/eierapfel/items/ad607679a9c80e9516e0
    // ＞要確認．おそらく，コンストラクタ は，純粋仮想関数や仮想関数を呼べない．

    // division: 分周(1,8,64,256,1024)
    // 分周の設定
    // set_division(division);
    
    // // 高速PWM
    // set_PWM_mode();
    
    // // カウンタ値の設定
    // set_OCRA(OCRA);
    // set_OCRB(OCRB);
}

void Timer0::set_PWM_mode(){
    // 高速PWM
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << WGM02);
}


void Timer0::set_outputA(int mode){
    //0:標準ポート動作 (OC1x切断)
    //1:比較一致でOC1Aピン トグル(交互)出力、OC1Bは標準ポート動作(OC1B切断)
    //2:比較一致でLow、BOTTOMでHighをOC1Aピンへ出力 (非反転動作)
    //3:比較一致でHigh、BOTTOMでLowをOC1Aピンへ出力 (反転動作)
    
    switch(mode){
        case 0:
            CBI(DDRD, DDD6);
            TCCR0A |= (0 << COM0A1) | (0 << COM0A0);
            break;
        case 1:
            SBI(DDRD, DDD6);
            TCCR0A |= (0 << COM0A1) | (1 << COM0A0);
            break;
        case 2:
            SBI(DDRD, DDD6);
            TCCR0A |= (1 << COM0A1) | (0 << COM0A0);
            break;
        case 3:
            SBI(DDRD, DDD6);
            TCCR0A |= (1 << COM0A1) | (1  << COM0A0);
            break;
    }
}

void Timer0::set_outputB(int mode){
    //0:標準ポート動作 (OC1x切断)
    //1:比較一致でOC1Aピン トグル(交互)出力、OC1Bは標準ポート動作(OC1B切断)
    //2:比較一致でLow、BOTTOMでHighをOC1Bピンへ出力 (非反転動作)
    //3:比較一致でHigh、BOTTOMでLowをOC1Bピンへ出力 (反転動作)
    
    switch(mode){
        case 0:
            CBI(DDRD, DDD5);
            TCCR0A |= (0 << COM0B1) | (0 << COM0B0);
            break;
        case 1:
            SBI(DDRD, DDD5);
            TCCR0A |= (0 << COM0B1) | (1 << COM0B0);
            break;
        case 2:
            SBI(DDRD, DDD5);
            TCCR0A |= (1 << COM0B1) | (0 << COM0B0);
            break;
        case 3:
            SBI(DDRD, DDD5);
            TCCR0A |= (1 << COM0B1) | (1 << COM0B0);
            break;
    }

}


void Timer0::set_division(int division){
    // 分周の設定
    this->division=division;
    switch(division){
        case 1: // stp = 0.125us(1分周）
            TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);
        break;

        case 8: // stp = 1us(8分周）
            TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);
        break;

        case 64: // stp = 8us(64分周）
            TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);
        break;

        case 256:// stp = 32us(256分周）
            TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00);
        break;

        case 1024: // stp = 0.128 ms(1024分周）
            TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
        break;

        default://機能停止
            TCCR0B |= (0 << CS02) | (0 << CS01) | (0 << CS00);
        break;
    }
    
}

// タイマー割り込みの有効化
void Timer0::set_interrupt(int interrupt_type){
    // interrupt  (0:TOIE1, 1:OCIE1A, 2: OCIE1B
    //タイマー割り込みの許可
    switch (interrupt_type){
    case 0:
        this->interrupt_period=0.125*division*OCR1A;//高速PWM以外だとTOP値が変わるので注意
        TIMSK0 |= (0<< TOIE0);//TOIE1割り込みを利用>>TIMER1_OVF_vect
        break;
    case 1:
        this->interrupt_period=0.125*division*OCR1A;
        TIMSK0 |= (1<< OCIE0A);//OCIE1A割り込みを利用>>TIMER1_COMPA_vect
        break;
    case 2:
        this->interrupt_period=0.125*division*OCR1B;
        TIMSK0 |= (1<< OCIE0B);//OCIE1B割り込みを利用>>TIMER1_COMPB_vect
        break;
    
    default:
        break;
    }
}

void Timer0::start_interrupt(){
    sei();
}