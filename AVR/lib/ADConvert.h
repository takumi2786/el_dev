class ADConvert{
private:
    // プロパティ
	double AD_val;//ADCの変換値
	double ValMax;
	void start_ADC();
	void update_ADC_val();

public:
	// メゾッド
	ADConvert(int AD_PIN,double ValMax);
	// AD変換値を出力
	double read_ADC_val();
};


