class SpeedMeter{
private:

	int passing_flg;//車輪が通過中かどうか
	// double sensor_val;//センサー出力電圧の値[V]
	double speed_val;// 時速 [km/h]
	double wheel_len=1.0;//自転車の車輪径[m]
	double time_val;//[s]



	// クラス
	ADConvert* adc;

	double time_stp;//実行感覚


	void calculate_speed();

	// 数字を７セグモジュールで表示
	void disp_number(int N);

public:
	SpeedMeter(ADConvert* adc_,double time_stp);

	// 車輪の状態を読み，値を表示
	double read_speed();


	



};
