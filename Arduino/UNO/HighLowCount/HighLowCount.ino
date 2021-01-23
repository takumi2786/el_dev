//リモコンのPWMをカウント
//入力：リモコンの赤外線信号
//出力：High時間,LOW時間

//寮のリモコン
//フォーマット：家生協
//その他：
//信号は，二回繰り返される．
//8bit * 18 = 144 bit が二回繰り返される．

#define SENSOR 2
int state;//現在の受信信号の状態 //※受信側のHIGHは、送信側とは逆になることに注意
int state_bef;//一つ前の状態
unsigned long t_change;//立ち上がり開始時間
unsigned long t;//時間

void setup() {
  Serial.begin(4800);
  Serial.println("Start");
 
}
void weight_while_signal_on(){
  while(!digitalRead(SENSOR)==HIGH);
}

void weight_while_signal_off(){
  while(!digitalRead(SENSOR)==LOW);
}


void loop() {
  //  何も受信していない場合、待つ
  weight_while_signal_off();
  state_bef=HIGH;
  // 切り替わり時間
  t_change = micros();
  t=0;
  while(t<100000){
    state = !digitalRead(SENSOR);
    //時刻取得
    t=micros()-t_change;
    
    
    //状態が切り替わったら
    if(state != state_bef){
      //切り替わり時間
      t_change = micros();
      Serial.print(t);
      if (state_bef == HIGH)Serial.print(",");
      else Serial.print("\n");
    }

    state_bef=state;
  }
  Serial.print("end\n");
}
