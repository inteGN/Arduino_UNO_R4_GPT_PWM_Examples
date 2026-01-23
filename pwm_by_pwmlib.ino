//************************************************
//  FILE        :pwm_by_pwmlib.ino
//  DATE        :2026/01/20
//  DESCRIPTION :PWM output by pwm.h library
//  BOARD TYPE  :UNO R4 MINIMA
//  AUTHER      :inteGN
//************************************************
/*
D2ピン、D3ピン、D11ピンのいずれかにPWM信号を出力します。
 - pwmD2.begin()のみをアクティブにすると、PWMパルスはD2ピンから出力されます。
   しかしArduino公式はD2ピンがPWM信号を出力できるとはしていません。
 - pwmD3.begin()のみをアクティブにすると、PWMパルスはD3ピンから出力されます。
 - pwmD11.begin()のみをアクティブにすると、PWMパルスはD11ピンから出力されます。
 - いずれか2つをアクティブにすると、どのピンもPWM信号を出力しなくなります。

This program output PWM signal to pin D2, D3 or D11.
 - If pwmD2.begin() is only activated PWM pulse is output from pin D2,
   but Arduino official does not mention D2 can output PWM signal.
 - If pwmD3.begin() is only activated PWM pulse is output from pin D3.
 - If pwmD11.begin() is only activated PWM pulse is output from pin D11.
 - When any two are activated, no pins output a PWM signal.
*/


//// Includes
#include <Arduino.h>
#include "pwm.h"

//// Grobals
PwmOut    pwmD2(D2);                        //Arduino pin D2 / RA4M1 P105 GPT1_A
PwmOut    pwmD3(D3);                        //Arduino pin D3 / RA4M1 P104 GPT1_B
PwmOut    pwmD11(D11);                      //Arduino pin D11 / RA4M1 P109 GPT1_A
uint32_t  pwm_reriod = 12;
uint32_t  pwm_count0 =  3;                  //duty ratio 25 %

//// Setup function
void setup() {

  pwmD2.begin(pwm_reriod, pwm_count0, true, TIMER_SOURCE_DIV_256);
  //pwmD3.begin(pwm_reriod, pwm_count0, true, TIMER_SOURCE_DIV_256);
  //pwmD11.begin(pwm_reriod, pwm_count0, true, TIMER_SOURCE_DIV_256);

}

//// Loop function
void loop() {
// nothing to do
  delayMicroseconds(10);
}


