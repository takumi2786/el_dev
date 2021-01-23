class Servo{

public:
	Servo(ABTimer* timer);
    void change_deg(int deg);
    

private:
    int T0;//0degの時のパルス幅(us)
    int T180;//180degの時のパルス幅(us)
	ABTimer* timer;// 16bitタイマ-のポインタ 
    int calculate_T_from_deg(double deg);

};

// // コンストラクタ 
// Servo::Servo(){
//     // 基準パルス(us)
//     this->T0=500;
//     this->T180=2400;

//     // タイマーの初期設定
//     int division=8;
// 	int T_base = 20000;//20ms
// 	int T_on =  T180;//2.4ms(0deg)
//     *timer16 = Timer16(division,T_base,T_on);//1us

//     // 出力設定
//     timer16->set_outputB(2);
// }

// void Servo::change_deg(int deg){//角度に対応するパルス幅に変更 > OCR1Bを変える
//     int T_ = calculate_T_from_deg(deg);
//     // int T_ = 500;
//     // disp_number(T_/10);
//     timer16->set_OCR1B(T_);
// }



// int Servo::calculate_T_from_deg(double deg){
    
//     return ( (T180-T0)/180 )*deg+T0;
// }





