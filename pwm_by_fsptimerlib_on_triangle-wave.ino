//************************************************
//  FILE        :pwm_by_fsptimerlib_on_triangle-wave.ino
//  DATE        :2026/01/20, 2026/02/22
//  DESCRIPTION :PWM output by FspTimer.h library on triangle-wave
//  BOARD TYPE  :UNO R4 MINIMA
//  AUTHER      :inteGN
//************************************************
/*
このプログラムは、GPT0を用いてD6とD7ピンにデッドタイム付きの相補PWM信号を出力します。
GPT0はArduinoコアライブラリによって通常のPWMモードにリザーブされているので、begin()した後に
三角波PWMモードにするための各種の設定をしています。
チャネルAとBは、反対のエッジ動作をする正相と逆相を出力するよう設定されます。
チャネルAのデューティ比はloop()の中で連続的に変更され、チャネルBは指定したデッドタイムが
自動的に付加され設定されます。

This program uses GPT0 to generate complementary PWM signals with dead time on pins D6 and D7.
Because GPT0 is reserved by the Arduino core library for standard PWM mode, the program reconfigures it
for triangle‑wave PWM mode after calling begin().
Channels A and B are configured to output an inverted‑phase waveform, respectively, with opposite edge behavior.
The duty ratio of channel A is continuously updated inside loop(), while channel B is automatically updated
by the hardware with the specific dead time value.
*/


//// Includes
#include <Arduino.h>
#include "FspTimer.h"

//// Grobals
FspTimer  timer0;
gpt_gtior_setting_t gptGtior_set = {0};
uint32_t  pwm_period = 1200;
uint32_t  pwm_count0 = 600;

//// Setup function
void setup() {
//Setup serial port
  Serial.begin(115200);
  delay(2000);
//timer configurate
  gptGtior_set.gtior_b.gtioa = 0x07;                //set LOW initially, LOW at cycle end and toggle at compare-match 
  gptGtior_set.gtior_b.gtiob = 0x0B;                //set LOW initially, HIGH at cycle end and toggle at compare-match 
  gptGtior_set.gtior_b.oae = 1;                     //enable A output
  gptGtior_set.gtior_b.obe = 1;                     //enable B output
  bool rv = timer0.begin(TIMER_MODE_PWM, GPT_TIMER, 0, pwm_period, pwm_count0, TIMER_SOURCE_DIV_1);
  if (rv) {
    timer0.add_pwm_extended_cfg();
    auto cfg = timer0.get_cfg();
    auto ext = (gpt_extended_cfg_t*)cfg->p_extend;
    auto pwm_cfg = (gpt_extended_pwm_cfg_t*)ext->p_pwm_cfg;
    cfg->mode = TIMER_MODE_TRIANGLE_WAVE_SYMMETRIC_PWM;
    ext->gtior_setting.gtior = gptGtior_set.gtior;
    pwm_cfg->dead_time_count_up = 80;
    pwm_cfg->dead_time_count_down = 0;              //RA4M1 does not use dead_time_count_down
    timer0.open();
    timer0.set_duty_cycle(pwm_count0, CHANNEL_A);   //set after GPT0 is configurated by open()
    timer0.start();
    delayMicroseconds(100);                         //wait for 2 cycle of PWM
//gpio output configurate, only IOPORT_PERIPHERAL_GPT1 is usable here
    pinPeripheral(D6, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_GPT1));     //GPT0_B output
    pinPeripheral(D7, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_GPT1));     //GPT0_A output
  }
//dump GPT registers
  Serial.print("begin() success status is "); Serial.println(rv);
  Serial.print("R_GPT0->GTCR_b.MD: "); Serial.println(R_GPT0->GTCR_b.MD);
  Serial.print("R_GPT0->GTSTR_b.CSTRT0: "); Serial.println(R_GPT0->GTSTR_b.CSTRT0);
  Serial.print("R_GPT0->GTDTCR_b.TDE: "); Serial.println(R_GPT0->GTDTCR_b.TDE);  
  Serial.print("R_GPT0->GTIOR: "); Serial.println(R_GPT0->GTIOR, HEX);
  Serial.println();
}

//// Loop function
void loop() {
  for (int i=-500;i<500;i++) {
    timer0.set_duty_cycle(pwm_count0 + i, CHANNEL_A);
    delay(1);
  }
  for (int i=500;i>-500;i--) {
    timer0.set_duty_cycle(pwm_count0 + i, CHANNEL_A);
    delay(1);
  }
}


