//リモコンのPWMをカウント 時系列データを取得 横軸：時間　縦軸：電圧
//入力：リモコンの赤外線信号
//出力：High時間,LOW時間
//フォトダイオード：光を受けると，v_outがlowになる
#include "Arduino.h"
#define SENSOR 2

#define stp 50//us サンプルタイム
#define N 650//データ数


void setup() {
  Serial.begin(115200);
  Serial.println("Start");
}

void weight_while_signal_on(){
  while(!digitalRead(SENSOR)==HIGH);
}

void weight_while_signal_off(){
  while(!digitalRead(SENSOR)==LOW);
}

void weight_while_signal_const(int state){
  while( !digitalRead(SENSOR)==state );
}


/*
// これだとメモリ不足になる　// しかし，ラグがないので精度がいい
//int state=1;
int state_vec[N];
void loop() {
 int i=0;
 //  何も受信していない場合、待つ
 weight_while_signal_off();
 while(i< N){
   state_vec[i]= !digitalRead(SENSOR);// 逆転
   i++;
   delayMicroseconds(stp);
 }
 
 // 結果を出力
 int j=0;
 while(j< N){
  Serial.print(j*stp);//us
  Serial.print(',');
  Serial.println(state_vec[j]);
  j++;
 }
 delay(1000);
}
*/


// 長く計測できるが，誤差がでかい
int state=1;
//int state_vec[N];
void loop() {
 int i=0;
 //  何も受信していない場合、待つ
 weight_while_signal_off();
 while(i< N){
   //state_vec[i]=digitalRead(SENSOR);
   //i++;
   state=digitalRead(SENSOR);
   Serial.print(i*stp);//us
   Serial.print(',');
   Serial.println(state);
   delayMicroseconds(stp);
   i++;
 }
 

 delay(1000);


}










//int state;//現在の受信信号の状態 //※受信側のHIGHは、送信側とは逆になることに注意
//int state_bef;//一つ前の状態
//unsigned long t_change;//立ち上がり開始時間
//unsigned long t;//時間
//void setup() {
//  Serial.begin(112500);
//  Serial.println("Start");
// 
//}
//void weight_untile_signal_on(int& state){
//  while(digitalRead(SENSOR)==HIGH);
//}
//
//
//void loop() {
//  //  何も受信していない場合、待つ
//  weight_untile_signal_on(state);
//  state_bef=LOW;
//  // 切り替わり時間
//  t_change = micros();
//  t=0;
//  while(t<20000){
//    state = digitalRead(SENSOR);
//    //時刻取得
//    t=micros()-t_change;
//    
//    
//    //状態が切り替わったら
//    if(state != state_bef){
//      //切り替わり時間
//      t_change = micros();
//      Serial.print(t);
//      if (state_bef == LOW)Serial.print(",");
//      else Serial.print("\n");
//    }
//
//    state_bef=state;
//  }
//  Serial.print("end\n");
//}
