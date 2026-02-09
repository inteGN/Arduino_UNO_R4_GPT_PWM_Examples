//************************************************
//  FILE        :pwm_by_fsptimerlib.ino
//  DATE        :2026/01/20
//  DESCRIPTION :PWM output by FspTimer.h library
//  BOARD TYPE  :UNO R4 MINIMA
//  AUTHER      :inteGN
//************************************************
/*
このプログラムは、GPT0タイマーを使ってD6ピンとD7ピンにPWM信号を出力します。
2つのチャネルのPWM信号は1つのタイマーから生成されるため、立ち上がりが同期します。
各チャネルのデューティ比は、loop()の中で独立して連続更新されます。

This program outputs two PWM signals on pins D6 and D7 using GPT0.
Because both channels are generated from a single timer, the rising edges of the two PWM signals are synchronized.
The duty ratio of each channel is updated independently and continuously inside the loop() function.
*/


//// Includes
#include <Arduino.h>
#include "FspTimer.h"

//// Grobals
FspTimer  timer0;
uint32_t  pwm_period = 2400;
uint32_t  pwm_count0 = 1200;
uint32_t  $TEST0;

//// Setup function
void setup() {
//Setup serial port
  Serial.begin(115200);
  delay(2000);
//timer configurate
  timer0.begin(TIMER_MODE_PWM, GPT_TIMER, 0, pwm_period, pwm_count0, TIMER_SOURCE_DIV_1);
  timer0.enable_pwm_channel(CHANNEL_A);                 //D6(P106) GPT0_B enable
  timer0.enable_pwm_channel(CHANNEL_B);                 //D7(P107) GPT0_A enable
  timer0.open();
  timer0.set_duty_cycle(pwm_count0, CHANNEL_A);
  timer0.set_duty_cycle(pwm_count0, CHANNEL_B);
  timer0.start();
  delayMicroseconds(100);                               //wait for 2 cycle of PWM
//gpio output configurate, only IOPORT_PERIPHERAL_GPT1 is usable here
  pinPeripheral(D6, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_GPT1));     //GPT0_B output
  pinPeripheral(D7, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_GPT1));     //GPT0_A output
//dump GPT registers
  Serial.print("R_GPT0->GTCR_b.MD: "); Serial.println(R_GPT0->GTCR_b.MD); 
  Serial.print("R_GPT0->GTIOR: "); Serial.println(R_GPT0->GTIOR, HEX);
  Serial.println();
}

//// Loop function
void loop() {
  for (int i=-1000;i<1000;i++) {
    timer0.set_duty_cycle(pwm_count0 + i, CHANNEL_A);
    timer0.set_duty_cycle(pwm_count0 + i / 2, CHANNEL_B);
    delay(1);
  }
  for (int i=1000;i>-1000;i--) {
    timer0.set_duty_cycle(pwm_count0 + i, CHANNEL_A);
    timer0.set_duty_cycle(pwm_count0 + i / 2, CHANNEL_B);
    delay(1);
  }
}


