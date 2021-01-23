//受け取ったデータを文字列として表示する

//入力：リモコンの赤外線信号
//出力：8ビットデータ列


//寮のリモコン
//フォーマット：家生協
//エアコンのスイッチオン信号：
//信号は，二回繰り返される．
//8bit * 18 = 144 bit が二回繰り返される．

#define SENSOR 2
#define TimeLimit 1500 //波形の長さがこれをこえたらタイムアウト
void setup() {
  Serial.begin(19200);
  Serial.println("Start");
}


void weight_while_signal_on(){
  while(!digitalRead(SENSOR)==HIGH);
}

void weight_while_signal_off(){
  while(!digitalRead(SENSOR)==LOW);
}
// 一定時間を超えるとタイムアウト
bool weight_while_signal_on(unsigned long t0){
  while(!digitalRead(SENSOR)==HIGH){
    if( TimeLimit < (micros()-t0) )return true;
  }

  return false;
}
// 一定時間を超えるとタイムアウト
bool weight_while_signal_off(unsigned long t0){
  while(!digitalRead(SENSOR)==LOW){
    
    if( TimeLimit < (micros()-t0) )return true;
  }
  return false;
}


void weight_while_signal_const(int state){
  while( !digitalRead(SENSOR)==state );
}



//01の配列ではなく，データとして記録する
//データ数が可変だと，これでは対応できない->対応できた
#define DataNum 18 //データ数
#define TimeLimit 1500 //波形の長さがこれをこえたらタイムアウト
#define LoopNum 2
unsigned long t0,th,tl;
unsigned long t;

int state=1;
int data[LoopNum][DataNum];


void loop() {

 
 // 家製協(AEHA)フォーマット, NECフォーマットはこれでできる．（ハズ）．

//  何も受信していない場合、待つ
weight_while_signal_off();

 for (int k=0;k<LoopNum;k++){//二回繰り返す
  // リーダーコードを待つ
  weight_while_signal_on();//High
  weight_while_signal_off();//Low
  bool break_flg =false;
  for (int j=0;j<DataNum;j++){//データの数だけ続ける
    data[k][j]=0;//値を初期化
     for(int i=0;i<8;i++){//8ビット
       // high時間を計測 t_h
       // low時間を計測  t_l
       // t_l > t_h なら true 違えば false
       t0=micros();
       break_flg=weight_while_signal_on(t0);//High
       th=micros()-t0;
       break_flg=weight_while_signal_off(t0+th);//Low
       tl=micros()-(t0+th);
  
    
       if(break_flg)break;
  
       if(2*th < tl)//true
          data[k][j] = data[k][j] >> 1 | 0b10000000;//Highの時 (右に一つずらして，一番右に1を加える).
       else //false
          data[k][j] = data[k][j] >> 1 | 0b00000000;//Lowの時 (右に一つずらして，一番右に0を加える).
     }
     if(break_flg)break;
  }
  //最後に一個出るHIGH信号を待つ
  weight_while_signal_on();//High
  //一番最後以外は，繰り返し待ちのLOWが一度入る．
  if(k!=1)weight_while_signal_off();//Low
  
 }




 for (int k=0;k<LoopNum;k++){//二回繰り返す
   for (int j=0;j<DataNum;j++){
     Serial.println(data[k][j],HEX);
   }
   Serial.println("end");
 }
 delay(1000);
}
