#include "Mouse.h"
#include "Pins.h"
#include <FastLED.h>

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
    left *= -1;

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
