#include "LedControl.h"
#include <SoftwareSerial.h>

const int JOY_push = 3;
const int JOY_X = 1; // analog
const int JOY_Y = 0; // analog ___ JOY ___
int count = 0;

const int DIN_PIN = 7;
const int CS_PIN = 6;
const int CLK_PIN = 5;
int i = 0;
const byte EMOJI[][8] = { // http://xantorohara.github.io/led-matrix-editor/
  {
    B00111100,
    B01111110,
    B11011011,
    B11011011,
    B11111111,
    B10111101,
    B11000011,
    B01111110
  }, {
     B00000000,
    B00100100,
    B00100100,
    B00100100,
    B00000000,
    B01000010,
    B00111100,
    B00000000
  }, // emoji 0~1
  {
    B00011000,
    B00110001,
    B01100011,
    B11000110,
    B11000110,
    B01100011,
    B00110001,
    B00011000
  },{
    B00110001,
    B01100011,
    B11000110,
    B10001100,
    B10001100,
    B11000110,
    B01100011,
    B00110001
  },{
    B01100011,
    B11000110,
    B10001100,
    B00011000,
    B00011000,
    B10001100,
    B11000110,
    B01100011
  },{
    B11000110,
    B10001100,
    B00011000,
    B00110001,
    B00110001,
    B00011000,
    B10001100,
    B11000110
  },{
    B10001100,
    B00011000,
    B00110001,
    B01100011,
    B01100011,
    B00110001,
    B00011000,
    B10001100
  }, // left 2~6
  {
    B00011000,
    B10001100,
    B11000110,
    B01100011,
    B01100011,
    B11000110,
    B10001100,
    B00011000
  },{
    B10001100,
    B11000110,
    B01100011,
    B00110001,
    B00110001,
    B01100011,
    B11000110,
    B10001100
  },{
    B11000110,
    B01100011,
    B00110001,
    B00011000,
    B00011000,
    B00110001,
    B01100011,
    B11000110
  },{
    B01100011,
    B00110001,
    B00011000,
    B10001100,
    B10001100,
    B00011000,
    B00110001,
    B01100011
  },{
    B00110001,
    B00011000,
    B10001100,
    B11000110,
    B11000110,
    B10001100,
    B00011000,
    B00110001
  } // right 7~11
  ,
  {
    B00111100,
    B01000010,
    B10100101,
    B10011001,
    B10011001,
    B10100101,
    B01000010,
    B00111100
  } // cancle 12
  ,
  {
      B11111111,
      B11111111,
      B10111101,
      B10011001,
      B10000001,
      B10000001,
      B11111111,
      B00000000
    }
/*
  {
    B00010000,
    B00110000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00111000
  }, {
    B00111000,
    B01000100,
    B00000100,
    B00000100,
    B00001000,
    B00010000,
    B00100000,
    B01111100
  }, {
    B00111000,
    B01000100,
    B00000100,
    B00011000,
    B00000100,
    B00000100,
    B01000100,
    B00111000
  }, {
    B00000100,
    B00001100,
    B00010100,
    B00100100,
    B01000100,
    B01111100,
    B00000100,
    B00000100
  }, {
    B01111100,
    B01000000,
    B01000000,
    B01111000,
    B00000100,
    B00000100,
    B01000100,
    B00111000
  }, {
    B00111000,
    B01000100,
    B01000000,
    B01111000,
    B01000100,
    B01000100,
    B01000100,
    B00111000
  }, {
    B01111100,
    B00000100,
    B00000100,
    B00001000,
    B00010000,
    B00100000,
    B00100000,
    B00100000
  }, {
    B00111000,
    B01000100,
    B01000100,
    B00111000,
    B01000100,
    B01000100,
    B01000100,
    B00111000
  }, {
    B00111000,
    B01000100,
    B01000100,
    B01000100,
    B00111100,
    B00000100,
    B01000100,
    B00111000
  }, {
    B00111000,
    B01000100,
    B01000100,
    B01000100,
    B01000100,
    B01000100,
    B01000100,
    B00111000
  }
*/
};
const int IMAGES_LEN = sizeof(EMOJI) / 8;
LedControl display = LedControl(DIN_PIN, CLK_PIN, CS_PIN); // (DIN, CS, CLK, 1dot) ___DOT___

const int DIN_PIN_ = 11;
const int CS_PIN_ = 10;
const int CLK_PIN_ = 9;
LedControl display_ = LedControl(DIN_PIN_, CLK_PIN_, CS_PIN_);


void setup() {
  Serial.begin(9600);

  display.shutdown(0, false);
  display.setIntensity(0, 5); // LED 밝기(0~15)
  display.clearDisplay(0); // ___DOT___

  display_.shutdown(0, false);
  display_.setIntensity(0, 5); // LED 밝기(0~15)
  display_.clearDisplay(0); // ___DOT___

  pinMode(JOY_push, INPUT); // ___ JOY ___
}

void displayImage(const byte* image) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(image[i], 7 - j));
      display_.setLed(0, i, j, bitRead(image[i], 7 - j));
    }
  }
}

char why;
void loop() {
  int joy_push = digitalRead(JOY_push);                       // 변수 push에 디지털 3번핀에 입력되는 신호를 대입
  int joy_x = analogRead(JOY_X);                           // 변수 X에 아날로그 1번핀에 입력되는 신호를 대입
  int joy_y = analogRead(JOY_Y);                           // 변수 Y에 아날로그 0번핀에 입력되는 신호를 대입


  if(joy_x > 1000) { // left
    while(joy_x > 1000){
      for(int i = 7; i <= 11 ; i++) {
        displayImage(EMOJI[i]);
        delay(100);
        joy_x = analogRead(JOY_X);
        if(joy_x < 1000){ break;}
      }
      int joy_x = analogRead(JOY_X); 
    }
  }

  if(joy_x < 100) { // right
    while(joy_x < 100) {
      for(int i = 2; i <= 6; i++) {
        displayImage(EMOJI[i]);
        delay(100);
        joy_x = analogRead(JOY_X);
        if(joy_x > 100){ break;}
      }
      int joy_x = analogRead(JOY_X); 
    }
  }

  if(joy_y > 1000) { // smile (Down)
    while(joy_y > 1000) {
      joy_y = analogRead(JOY_Y);
      displayImage(EMOJI[0]);
      delay(100);
      if(joy_y < 1000){ break;}
      }
  }

  if(joy_y < 100) { // kill (Up)
    while(joy_y < 100) {
      joy_y = analogRead(JOY_Y);
      displayImage(EMOJI[12]);
      delay(100);
      if(joy_y > 100){ break;}
      }
  }

  displayImage(EMOJI[1]); // default

}
