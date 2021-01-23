// これめっちゃ使える．
#define SBI(reg,bit) reg |= (1 << bit)
#define CBI(reg,bit) reg &= ~(1 << bit)
#define HIGH 1
#define LOW 0

void init_display(){
	// 2桁表示モジュールへの入力用
	DDRD =0b01111111;
	PORTD=0b01111111;

}

// 数字を７セグモジュールで表示
void disp_number(int N){
	PORTD=0b01111111;//0に戻す
	for (int i = 0; i <= 6; ++i){
		// ibit目取得
		if(0b0001<<i & N) CBI(PORTD, i);
	}
}






