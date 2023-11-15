
#include <FastLED.h>

#define LED_PIN 7 //PE6

#define MSLEEP_PIN 12 // PD6
#define MOTOR_A1 6 // PD7
#define MOTOR_A2 9 // PB4
#define MOTOR_B1 5 // PC6
#define MOTOR_B2 10 // PCPB6

//#define IREMIT_FORWARD //RXLED //PB0
#define IREMIT_SIDE 13 // PC7
#define IREMIT_ANGLE 11 // PB7

#define SEN_LF A5 // PF0
#define SEN_LA A4 // PF1
#define SEN_LS A3 // PF4
#define SEN_RS A2 // PF5
#define SEN_RA A1 // PF6
#define SEN_RF A0 // PF7

#define MAG_ENCA 4 // PD4
//#define MAG_ENCB //TXLED // PD5

CRGB addressable_led[1];

class Mouse {
public:
  Mouse(void);
  void run_motors(int left, int right);
  void enable_ir(int direction);
  void disable_ir(int direction);
};

Mouse::Mouse(void)
{
    pinMode(MSLEEP_PIN, OUTPUT);
    digitalWrite(MSLEEP_PIN, 0);

    // conigure the motor drive pins.
    pinMode(MOTOR_A1, OUTPUT);
    pinMode(MOTOR_A2, OUTPUT);
    pinMode(MOTOR_B1, OUTPUT);
    pinMode(MOTOR_B2, OUTPUT);
    digitalWrite(MOTOR_A1, 0);
    digitalWrite(MOTOR_A2, 0);
    digitalWrite(MOTOR_B1, 0);
    digitalWrite(MOTOR_B2, 0);
    digitalWrite(MSLEEP_PIN, 1);

    // ir emitters
    //digitalWrite(IREMIT_FORWARD, OUTPUT);
    pinMode(IREMIT_ANGLE, OUTPUT);
    pinMode(IREMIT_SIDE, OUTPUT);
    //pinMode(IREMIT_FORWARD, 0);
    PORTB &= ~(1<<0);  // turn-off forward led 
    digitalWrite(IREMIT_ANGLE, 0);
    digitalWrite(IREMIT_SIDE, 0);

    // ir sensors
    pinMode(SEN_LF,INPUT);
    pinMode(SEN_LA,INPUT);
    pinMode(SEN_LS,INPUT);
    pinMode(SEN_RS,INPUT);
    pinMode(SEN_RA,INPUT);
    pinMode(SEN_RF,INPUT);
}
  
void Mouse::run_motors(int left, int right)
{
    if (left == 0)
    {
      digitalWrite(MOTOR_A1, 0);
      digitalWrite(MOTOR_A2, 0);
    } else if (left > 0)
    {
      digitalWrite(MOTOR_A1, 0);
      analogWrite(MOTOR_A2, left);
    } else
    {
      digitalWrite(MOTOR_A1, -left);
      analogWrite(MOTOR_A2, 0);
    }
    if (right == 0)
    {
      digitalWrite(MOTOR_B1, 0);
      digitalWrite(MOTOR_B2, 0);
    } else if (right > 0)
    {
      digitalWrite(MOTOR_B1, 0);
      analogWrite(MOTOR_B2, right);
    } else
    {
      digitalWrite(MOTOR_B1, -right);
      analogWrite(MOTOR_B2, 0);
    }
}

void Mouse::enable_ir(int direction)
{
  if (direction == 1)
  {
    PORTB |= 1<<0;
  } else if (direction == 2)
  {
    PORTC |= 1<<7;
  } else if (direction == 3)
  {
    PORTB |= 1<<7;
  }
}

void Mouse::disable_ir(int direction)
{
  if (direction == 1)
  {
    PORTB &= ~(1<<0);
  } else if (direction == 2)
  {
    PORTC &= ~(1<<7);
  } else if (direction == 3)
  {
    PORTB &= ~(1<<7);
  }
}

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
  mouse.run_motors(64,64);
  delay(500);

  mouse.run_motors(0,0);
  addressable_led[0] = CRGB::Black;
  FastLED.show();
  delay(2000);
  
  addressable_led[0] = CRGB::Red;
  FastLED.show();
  mouse.run_motors(-64,-64);
  delay(500);

  mouse.run_motors(0,0);
  addressable_led[0] = CRGB::Black;
  FastLED.show();
  delay(2000);
}

void setup() { 
    // configure the addressable led
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(addressable_led, 1);


//    Serial.begin(19200);
}

void loop()
{
  motor_loop();
}
