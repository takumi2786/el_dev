#ifndef INCLUDE_GUARD_DCRead
#define INCLUDE_GUARD_DCRead

#include <avr/io.h>

// デジタル値読み込みクラス
#define B 0
#define C 1
#define D 2
class DGRead{
public:
    DGRead(int Symbol, int PinNum){
        this->Symbol=Symbol;
        this->PinNum=PinNum;


        // ピンの初期化
        switch(this->Symbol){
            case B:
                this->DDR = &DDRB;
                this->PORT = &PORTB;
                this->PIN= &PINB;
            break;
            case C:
                this->DDR = &DDRC;
                this->PORT = &PORTC;
                this->PIN= &PINC;
            break;
            case D:
                this->DDR = &DDRD;
                this->PORT = &PORTD;
                this->PIN= &PIND;
            break;
            default:
            break;
        }
        CBI(*DDR,PinNum);
        SBI(*PORT,PinNum);
    }

    int read(){
        int dc_val=0; 
        dc_val = *PIN & (0b01 << PinNum);//C0ピンを読み込み
        return dc_val;
    }


private:
    int PinNum;
    int Symbol;
    volatile uint8_t* DDR;
    volatile uint8_t* PORT;
    volatile uint8_t* PIN;
};

#endif