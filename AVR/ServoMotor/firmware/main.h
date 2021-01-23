// これめっちゃ使える．
#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)

// 数字を７セグモジュールで表示
void disp_number(int N){
	PORTD=0b01111111;
	for (int i = 0; i <= 6; ++i){
		// ibit目取得
		if(0b0001<<i & N) CBI(PORTD, i);
	}
}






