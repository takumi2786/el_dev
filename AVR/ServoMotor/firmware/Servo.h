class Servo{

public:
	Servo(ABTimer* timer);
    void change_deg(int deg);//角度を変更する関数．
    

private:
    int T0;//0degの時のパルス幅(us)
    int T180;//180degの時のパルス幅(us)
	ABTimer* timer;// 16bitタイマ-のポインタ 
    int calculate_T_from_deg(double deg);//角度から，パルス幅を決定する関数．
};

