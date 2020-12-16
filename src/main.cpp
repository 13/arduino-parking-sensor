/* 
 8x8 Matrix:
   pin 13 = DataIn 
   pin 12 = CLK 
   pin 11 = LOAD/CS
 HC-SR04:
   pin 10 = TRIGGER
   pin 09 = ECHO
*/
#include <Arduino.h>
#include <LedControl.h>
#include <NewPing.h>

#define PING_PIN 10
#define TRIGGER_PIN 10 
#define ECHO_PIN 9
#define MAX_DISTANCE 350 
#define MY_DISTANCE 20

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
LedControl lc = LedControl(13,12,11,1);

unsigned long time = 0;
long timeout = 500;
int state = 0;

void setup() {
  Serial.begin (9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

}

  byte smiley[8]={
      B00111100,
      B01000010,
      B10100101,
      B10000001,
      B10100101,
      B10011001,
      B01000010,
      B00111100
  };
  
  byte ex[8]={
      B10000001,
      B01000010,
      B00100100,
      B00011000,
      B00011000,
      B00100100,
      B01000010,
      B10000001
  };  
  
  byte arrow[8]={
      B00010000,
      B00101000,
      B01000100,
      B10000010,
      B00010000,
      B00101000,
      B01000100,
      B10000010
  };   

  byte null[8]={
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B00000000,
      B00000000
  };   
 
  byte number1[8]={
      B00000000,
      B00001000,
      B00011000,
      B00101000,
      B00001000,
      B00001000,
      B00011100,
      B00000000
  };   
  byte number2[8]={
      B00000000,
      B00011000,
      B00100100,
      B00001000,
      B00010000,
      B00100000,
      B00111100,
      B00000000
  }; 
  byte number3[8]={
      B00000000,
      B00111100,
      B00000100,
      B00011000,
      B00000100,
      B00100100,
      B00011000,
      B00000000
  };
  byte number4[8]={
      B00000000,
      B00000100,
      B00001100,
      B00010100,
      B00111100,
      B00000100,
      B00000100,
      B00000000
  }; 
  byte number5[8]={
      B00000000,
      B00111100,
      B00100000,
      B00111000,
      B00000100,
      B00100100,
      B00011000,
      B00000000
  };   
  
void writeMatrix(byte bname[8]) {
  for (int i=0; i <= 7; i++){
    lc.setRow(0,i,bname[i]);
    delay(10);  
  }
}

void writeMatrixMirror(byte bname[8]) {
  for (int i=0; i <= 7; i++){
    lc.setRow(0,i,bname[7-i]);
    delay(10);  
  }
}

void loop() {
  delay(50); // 50
  unsigned int uS = sonar.ping(); 
  unsigned int distance = (uS / US_ROUNDTRIP_CM);
  
  if (distance == 0){
    writeMatrix(null);
    
  } else if (distance > MAX_DISTANCE) { 
    writeMatrix(null);
    
  } else if (distance > 0 && distance <= MY_DISTANCE) {
    if (state == 1){
      if (time < timeout) {
        writeMatrix(smiley);
        time = time + 1;
      } else {
        writeMatrix(null);
        time = timeout + 1;
      }
    } else {
      time = 0;
    }  
    state = 1;
    
  } else if (distance > MY_DISTANCE && distance <= MAX_DISTANCE) {
    if (state == 2){
      if (time < timeout) {
        if (distance < 50){
          if (distance < 30){
            writeMatrix(number1);
          } else if (distance < 40){
            writeMatrix(number2);
          } else if (distance < 50){
            writeMatrix(number3);
          }
        } else {
          writeMatrix(arrow);
        }
        time = time + 1;
      } else {
        writeMatrix(null);
        time = timeout + 1;
      }
    } else {
      time = 0;
    }  
    state = 2;
    
  }

  // debug
  /*Serial.print("Dist: ");
  Serial.print(distance);
  Serial.println("");
  Serial.print("Time: ");
  Serial.print(time);
  Serial.println("");
  Serial.print("Stat: ");
  Serial.print(state);
  Serial.println("");
  Serial.println("");*/
}
