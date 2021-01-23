//読み取った波形と，持っているデータが一致した場合に処理
#define SENSOR 2
#define DataNum 12 //データ数
#define TimeLimit 1500 //波形の長さがこれをこえたらタイムアウト
void setup() {
  Serial.begin(112500);
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



//01の配列ではなく，データとして記録する v1
//データ数が可変だと，これでは対応できない v2
//->対応できた v3
// 特定のデータを送られた時だけ処理をする．


unsigned long t0,th,tl;
unsigned long t;

int state=1;
int data[DataNum];

// ここに読み取る情報を書く
int data_ref[DataNum]={ 
  0x14,
  0x63,
  0x0,
  0x10,
  0x10,
  0xFE,
  0x9,
  0x30,
  0x61,
  0x4,
  0x12,
  0x0
};

void loop() {

 //  何も受信していない場合、待つ
 weight_while_signal_off();
  
 // リーダーコードを待つ
 weight_while_signal_on();//High
 weight_while_signal_off();//Low

 // high時間を計測 t_h
 // low時間を計測  t_l
 // t_l > t_h なら true 違えば false
 // 家製協(AEHA)フォーマット, NECフォーマットはこれでできる．（ハズ）．
bool break_flg =false;
for (int j=0;j<DataNum;j++){//データの数だけ続ける
   for(int i=0;i<8;i++){//8ビット
     t0=micros();
     break_flg=weight_while_signal_on(t0);//High
     th=micros()-t0;
     break_flg=weight_while_signal_off(t0+th);//Low
     tl=micros()-(t0+th);
  
     if(break_flg)break;

     if(2*th < tl)//true
        data[j] = data[j] >> 1 | 0b10000000;//Highの時 (右に一つずらして，一番右に1を加える).
     else //false
        data[j] = data[j] >> 1 | 0b00000000;//Lowの時 (右に一つずらして，一番右に0を加える).
   }

   if(break_flg)break;
}

//// 表示
//for (int j=0;j<DataNum;j++){
// Serial.println(data[j],HEX);
//}

bool is_same_flg=true;
for (int j=0;j<DataNum;j++){
 if(data[j]!=data_ref[j]){
   is_same_flg=false;
   break;
 }
}

if(is_same_flg)Serial.println("Same");
else Serial.println("Not Same");

Serial.println("end");
 
 delay(1000);
}
