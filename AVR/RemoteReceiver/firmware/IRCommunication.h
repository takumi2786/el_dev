#ifndef INCLUDE_GUARD_IRCommunication
#define INCLUDE_GUARD_IRCommunication

#include "TimeCounter.h"
#include "DGRead.h"
#define DataNum 18
#define LoopNum 1

class IRCommunication{
public:
	IRCommunication(TimeCounter* TC_, DGRead* DGR_);
	void read_data(int* data);
	bool judge_data(int* data);
	void disp_data(int* data);
	void update_time_counter();// タイマー割り込み内で利用する関数

private:	
	TimeCounter* TC;//時間計測クラス
	DGRead* DGR;//デジタル信号読み取りクラス
	bool break_flg;

	void wait_while_signal_HIGH(int flg);
	void wait_while_signal_LOW(int flg);

	void wait_leader();

	bool read_1bit();
	void read_8bit(int* data,int j);
	
	int correct_data[DataNum]={// ONボタン 冷房
		0x23, //35
		0xCB, //75
		0x26, //38
		0x1,  //1
		0x0,  //0
		0x20, //32
		0x18, //24
		0x7,  //   2
		0x36,//24
		0x78,//73
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0xCE//78
	};
};

IRCommunication::IRCommunication(TimeCounter* TC_, DGRead* DGR_){
	TC = TC_;
	DGR = DGR_;
}

void IRCommunication::wait_while_signal_HIGH(int flg=false){
  int t0,t;
  t0 = TC->get_useconds();//開始時刻;
  while(1){
	//ノイズ防止
	//２回連続でLOWなら，ブレーク
	if( !DGR->read() ){
	   if( !DGR->read() )return;
	return;
	}
	if(flg){
		// タイムアウト
		t=TC->get_useconds();//開始時刻;
		if(t-t0 > 30000){
			break_flg=true;
			return;
		}
	}
  }
}

void IRCommunication::wait_while_signal_LOW(int flg=false){
  int t0,t;
  t0=TC->get_useconds();//開始時刻;
  while(1){
    //２回連続でHIGHなら，ブレーク
	if( DGR->read() ){
	   if( DGR->read() )return;
	}
	if(flg){
		t=TC->get_useconds();//開始時刻;
		if(t-t0 > 30000){
			break_flg=true;
			return;
		}
	}
  }
}

void IRCommunication::wait_leader(){
	// リーダーコードを待つ
	wait_while_signal_LOW(true);
	wait_while_signal_HIGH(true);
}

// 1ビット分信号を読む
bool IRCommunication::read_1bit(){
	int t0,th,tl;
	TC->start_TC();//タイマースタート

	t0 = TC->get_useconds();//開始時刻
	//LOW時間
	wait_while_signal_LOW(true);
	tl = TC->get_useconds()-t0;
	//HIGH時間
	wait_while_signal_HIGH(true);
	th = TC->get_useconds()-(tl+t0);

	if(tl*2 < th)return true;
	else return false;

	return false;
}

void IRCommunication::read_8bit(int* data,int j){
	int bit_c;
	data[j]=0;//値を初期化
	for(int i=0;i<8;i++){//8ビット
		bit_c = read_1bit();
		if(bit_c == true) data[j] = data[j] >> 1 | 0b10000000;//Trueの時，(右に一つずらして，一番右に1を加える)->
		else              data[j] = data[j] >> 1 | 0b00000000;//falseの時，(右に一つずらして，一番右に0を加える)->

		if(break_flg)break;
	}
}


bool IRCommunication::judge_data(int* data){
	bool NG_flg=false;
	for (int i = 0; i < DataNum; i++){
	// for (int i = 0; i < 7; i++){
		if( data[i] != correct_data[i])NG_flg=true;
	}
	return NG_flg;
}


void IRCommunication::disp_data(int* data){
	for (int i = 0; i < DataNum; i++){
		if(break_flg)return;
		disp_number(99);
		_delay_ms(20);
		// if( data[i] == correct_data[i]) disp_number(data[i]);
		// else disp_number(3);
		disp_number(data[i]);
		_delay_ms(500);
	}
}

void IRCommunication::read_data(int* data){
	break_flg=false;	
	wait_while_signal_HIGH(false);//何もしない間待つ

	wait_leader();//リーダーコードを待つ
	for (int j= 0; j < DataNum; j++){//データビットを読み込む
		read_8bit(&data[0],j);
		if(break_flg)break;
	}
	//最後に一個出るHIGH信号を待つ
	wait_while_signal_LOW(true);

}

// 割り込み内で利用する関数
void IRCommunication::update_time_counter(){
	TC->update_TC();
}

#endif