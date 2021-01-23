#include <avr/io.h>
#include <util/delay.h>

#define TRUE 1
#define FALSE 0
#define NULL '\0'

// ビットをオンオフするマクロ
#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)

int main() {
    int T=1000;
    while(1){
      SBI(PORTB,DDB0);
      _delay_ms(T);
      CBI(PORTB,DDB0);
      _delay_ms(T);
    }
    return 0;
}

