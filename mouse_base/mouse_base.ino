#include <FastLED.h>
#include "Pins.h"
#include "Mouse.h"

CRGB addressable_led[1];

Mouse mouse;

void sensor_loop()
{
  addressable_led[0] = CRGB::Blue;
  FastLED.show();
  mouse.enable_ir(1);
  delay(100);
  Serial.println(analogRead(SEN_LF));
  Serial.println(analogRead(SEN_RF));
  mouse.disable_ir(1);
  mouse.enable_ir(2);
  delay(100);
  Serial.println(analogRead(SEN_LS));
  Serial.println(analogRead(SEN_RS));
  mouse.disable_ir(2);
  mouse.enable_ir(3);
  delay(100);
  Serial.println(analogRead(SEN_LA));
  Serial.println(analogRead(SEN_RA));
  mouse.disable_ir(3);
  Serial.println("");
  addressable_led[0] = CRGB::Black;
  FastLED.show();
  delay(1000);
}

void motor_loop()
{  
  // enable the motor controller
  digitalWrite(MSLEEP_PIN, 1);
    
  addressable_led[0] = CRGB::Green;
  FastLED.show();
  mouse.run_motors(-255,255);//mouse.run_motors(64,64);
  delay(500);

  mouse.run_motors(0,0);
  addressable_led[0] = CRGB::Black;
  FastLED.show();
  delay(2000);
  
  addressable_led[0] = CRGB::Red;
  FastLED.show();
  mouse.run_motors(255,-255);//mouse.run_motors(-64,-64);
  delay(500);

  mouse.run_motors(0,0);
  addressable_led[0] = CRGB::Black;
  FastLED.show();
  delay(2000);
}

void setup() { 
    // configure the addressable led
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(addressable_led, 1);

    Serial.begin(9600);
}
int before = 0,after = 0;
void loop()
{
  //sensor_loop();
  //motor_loop();
  test();
}

void test()
{
  mouse.enable_ir(2);
  //delay(10);
  int LS = analogRead(SEN_LS);
  int RS = analogRead(SEN_RS);
  mouse.disable_ir(2);
  mouse.enable_ir(1);
  //delay(10);
  int turn = analogRead(SEN_LF);
  mouse.disable_ir(1);
  //Serial.println(turn);
  
  int var = 64 + (LS - RS)*2;
  
  if(turn>200){
    //Reverse, then turn
    //mouse.run_motors(-64,64);
    //delay(100);
    mouse.run_motors(-64,-64);
    //delay(200);
  }else{
    //Forward
    mouse.run_motors(64,-var);
  }
}
