#include <Servo.h>
Servo servo;
//int sw;
int angle=90;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(13);
  servo.write(angle);
   
}

void serialEvent(){
  
  byte sw=Serial.read();
  if(sw==0x0a)return;
  sw=sw-0x30;
  Serial.println(sw);
  angle=sw;

  return;
}

void loop() {
//  byte sw=Serial.read();
//  sw=sw-0x30;
  
//  if(sw==0){
//    sw=(int)sw;
//    angle=45;
//    Serial.println(sw);
//  }
//  if(sw==1){
//    angle=120;
//    Serial.println(sw);
//  }

  servo.write(angle);
  
//  servo.writeMicroseconds(1000);

}
