//************************************************
//  FILE        :knight_rider_by_analogwrite.ino
//  DATE        :2026/01/20
//  DESCRIPTION :PWM output by analogwrite using class
//  BOARD TYPE  :NANO R4
//  AUTHER      :inteGN
//************************************************
/*
このスケッチは、8つのLEDを使用して古典的なナイトライダーの発光アニメーションを実装しています。
主要なアイデアは、LEDの動作（点灯、増光、減光）を小さなクラス内にカプセル化し、
順番にトリガーされる8つの独立したインスタンスを作成したことです。
NANO R4では内部的にGPTタイマーを使用しますが、このプログラムではそのハードウェア詳細を隠蔽した
PWMインターフェイスであるanalogWrite()のみを使っています。

This sketch implements a classic Knight Rider light animation using eight LEDs.
The key idea is to encapsulate LED behavior—turn‑on, fade‑in, fade‑out—inside a small class, 
and then create eight independent instances that are triggered in sequence.
Although the NANO R4 uses the GPT hardware timer internally, the sketch relies only on analogWrite(), 
which provides PWM interface without exposing any hardware details. 
*/


//// Includes
#include <Arduino.h>

//// Class led blink with fade in and out
class led {
  public:
    void begin(uint8_t pin);
    void trig(uint32_t stimuTime);
    void fade(int32_t tauFadein, int32_t tauFadeout);
  private:
    uint8_t pinNum;
    uint8_t stimuNum = 0;
    int32_t intensity = 0;
};
void  led::begin(uint8_t pin) {                             //pin definition
  pinNum = pin;
  pinMode(pinNum, OUTPUT);
}
void  led::trig(uint32_t stimuTime) {                       //led stimulation
  stimuNum = stimuTime;
}
void  led::fade(int32_t tauFadein, int32_t tauFadeout) {    //led fade in and out
  if (stimuNum > 0) {
    intensity = intensity + (65535 - intensity) / tauFadein;
    if (intensity > 65535) {intensity = 65535;}
    stimuNum--;
  }
  else {
    intensity = intensity - intensity / tauFadeout;
    if (intensity < 0) {intensity = 0;}
  }
  analogWrite(pinNum, (intensity >> 8));
}

//// Definitions
#define   LED_PINS            8               //LEDs
#define   LED_PATTERNS        14              //blink patterns
#define   TRIG_MILLIS         100             //time to trigger next LED
#define   STIMU_DURATION      100             //LED stimulate duration
#define   TAU_FADEIN          50              //fade in time constant
#define   TAU_FADEOUT         100             //fade out time constant

//// Globals
led  ledPin[LED_PINS];
uint8_t pin[LED_PINS] = {2, 3, 4, 5, 6, 7, 8, 9};               //pin D2 D3 D4 D5 D6 D7 D8 D9
uint8_t order[LED_PATTERNS] = {0,1,2,3,4,5,6,7,6,5,4,3,2,1};    //Knight rider pattern
uint8_t ledNum = 0;
uint32_t millis_old;

//// Setup function
void setup() {
  for(int i=0;i<LED_PINS;i++) {
    ledPin[i].begin(pin[i]);
  }
  millis_old = millis();
}

//// Loop function
void loop() {
  if (millis() > (millis_old + TRIG_MILLIS)) {
    millis_old = millis();
    ledPin[order[ledNum]].trig(STIMU_DURATION);
    ledNum = (ledNum + 1) % LED_PATTERNS;
  }
  for(int i=0;i<LED_PINS;i++) {
    ledPin[i].fade(TAU_FADEIN, TAU_FADEOUT);
  }
  delay(1);
}

