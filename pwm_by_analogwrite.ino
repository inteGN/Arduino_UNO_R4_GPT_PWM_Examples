//************************************************
//  FILE        :pwm_by_analogwrite.ino
//  DATE        :2026/01/20
//  DESCRIPTION :PWM output by analogwrite
//  BOARD TYPE  :UNO R4 MINIMA
//  AUTHER      :inteGN
//************************************************
/*
D2ピン、D3ピンの両方にPWM信号を出力します。
analogWrite()は、コンペアマッチAとBの間で設定の競合を発生させません。
また、2つのPWMの立ち上がりパルスは、両方のチャネルが1つのGPTから
生成されるためパルスの立ち上がりは同期します。

This program outputs PWM signals on both D2 and D3 pins.
The analogWrite() function does not cause any configuration conflict between
compare-match A and compare-match B of the shared GPT timer.
The rising edges of the two PWM signals are synchronized because both channels
are generated from the same GPT instance.
*/


//// Includes
#include <Arduino.h>

//// Setup function
void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(D2, OUTPUT);                    //Arduino D2 pin / RA4M1 P105 GPT1_A
  pinMode(D3, OUTPUT);                    //Arduino D3 pin / RA4M1 P104 GPT1_B
  analogWrite(D2, 100);
  analogWrite(D3, 150);
  delay(100);                             //wait for GTCCR buffer propagation
  Serial.print("R_GPT0->GTSTR : "); Serial.println(R_GPT0->GTSTR);
  Serial.print("R_GPT0->GTPR  : "); Serial.println(R_GPT1->GTPR);
  Serial.print("R_GPT0->GTCCRA: "); Serial.println(R_GPT1->GTCCR[0]);
  Serial.print("R_GPT0->GTCCRB: "); Serial.println(R_GPT1->GTCCR[1]);
  delay(10000);                           //show waveforms
}

//// Loop function
void loop() {
  //Duty ratio up/down
  for (int i = 100; i < 200; i++) {
    analogWrite(D2, i);
    analogWrite(D3, i + 50);
    delay(30);
  }
  for (int i = 200; i >=100; i--) {
    analogWrite(D2, i);
    analogWrite(D3, i + 50);
    delay(30);
  } 
}


