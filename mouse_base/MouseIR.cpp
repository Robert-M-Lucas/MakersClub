#include "MouseIR.h"
#include "Pins.h"
#include <FastLED.h>

unsigned sub_or_zero(unsigned a, unsigned b) {
    if (a > b) {
        return 0;
    }
    return b - a;
}

// void IRReading::scale(unsigned maxValue) {
//     leftSide = constrain(map(leftSide, 0, maxValue, 0, 1024), 0, 1024);
//     leftAngled = constrain(map(leftAngled, 0, maxValue, 0, 1024), 0, 1024);
//     leftForward = constrain(map(leftForward, 0, maxValue, 0, 1024), 0, 1024);

//     rightSide = constrain(map(rightSide, 0, maxValue, 0, 1024), 0, 1024);
//     rightAngled = constrain(map(rightAngled, 0, maxValue, 0, 1024), 0, 1024);
//     rightForward = constrain(map(rightForward, 0, maxValue, 0, 1024), 0, 1024);
// }

void IRReading::serialPrintValues()
{
    Serial.print("Left Side: ");
    Serial.print(leftSide);
    Serial.print("| Left Angle: ");
    Serial.print(leftAngled);
    Serial.print("| Left Forward: ");
    Serial.print(leftForward);

    Serial.print("| Right Side: ");
    Serial.print(rightSide);
    Serial.print("| Right Angle: ");
    Serial.print(rightAngled);
    Serial.print("| Right Forward: ");
    Serial.println(rightForward);
}

IRReading IRReading::difference(IRReading before, IRReading after)
{
    return IRReading {
        sub_or_zero(before.leftSide, after.leftSide),
        sub_or_zero(before.leftAngled, after.leftAngled),
        sub_or_zero(before.leftForward, after.leftForward),

        sub_or_zero(before.rightSide, after.rightSide),
        sub_or_zero(before.rightAngled, after.rightAngled),
        sub_or_zero(before.rightForward, after.rightForward),
    };
}

void MouseIR::enable_ir(SensorDirection direction)
{
  // Forward
  if (direction == SensorDirection::Forward)
  {
    PORTB |= 1<<0;
  } 
  // Side
  else if (direction == SensorDirection::Side)
  {
    PORTC |= 1<<7;
  } 
  // Angle
  else if (direction == SensorDirection::Angle)
  {
    PORTB |= 1<<7;
  }
}


IRReading MouseIR::read_all_ir()
{
    unsigned leftSide = analogRead(SEN_LS);
    unsigned leftAngled = analogRead(SEN_LA);
    unsigned leftForward = analogRead(SEN_LF);

    unsigned rightSide = analogRead(SEN_RS);
    unsigned rightAngled = analogRead(SEN_RA);
    unsigned rightForward = analogRead(SEN_RF);

    return IRReading { leftSide, leftAngled, leftForward, rightSide, rightAngled, rightForward };
}

IRReading MouseIR::read_all_callibrated()
{
    IRReading before = read_all_ir();

    enable_ir(SensorDirection::Side);
    unsigned leftSide = analogRead(SEN_LS);
    unsigned rightSide = analogRead(SEN_RS);
    disable_ir(SensorDirection::Side);

    enable_ir(SensorDirection::Angle);
    unsigned leftAngled = analogRead(SEN_LA);
    unsigned rightAngled = analogRead(SEN_RA);
    disable_ir(SensorDirection::Angle);

    enable_ir(SensorDirection::Forward);
    unsigned leftForward = analogRead(SEN_LF);
    unsigned rightForward = analogRead(SEN_RF);
    disable_ir(SensorDirection::Forward);

    IRReading after = {leftSide, leftAngled, leftForward, rightSide, rightAngled, rightForward};

    return IRReading::difference(before, after);
}

void MouseIR::disable_ir(SensorDirection direction)
{
  if (direction == SensorDirection::Forward)
  {
    PORTB &= ~(1<<0);
  } else if (direction == SensorDirection::Side)
  {
    PORTC &= ~(1<<7);
  } else if (direction == SensorDirection::Angle)
  {
    PORTB &= ~(1<<7);
  }
}
