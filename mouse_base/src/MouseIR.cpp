#include <Arduino.h>
#include <FastLED.h>

#include "MouseIR.h"
#include "Pins.h"

unsigned sub_or_zero(const unsigned a, const unsigned b) {
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

void IRReading::serialPrettyPrintValues() const {
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

void IRReading::serialOutputValues() const {
    Serial.print(leftSide);
    Serial.print(";");
    Serial.print(leftAngled);
    Serial.print(";");
    Serial.print(leftForward);
    Serial.print(";");
    Serial.print(rightForward);
    Serial.print(";");
    Serial.print(rightAngled);
    Serial.print(";");
    Serial.print(rightSide);
    Serial.println();
}

IRReading IRReading::difference(const IRReading &before, const IRReading &after)
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

void MouseIR::enable_ir(const SensorDirection direction)
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
    const unsigned leftSide = analogRead(SEN_LS);
    const unsigned leftAngled = analogRead(SEN_LA);
    const unsigned leftForward = analogRead(SEN_LF);

    const unsigned rightSide = analogRead(SEN_RS);
    const unsigned rightAngled = analogRead(SEN_RA);
    const unsigned rightForward = analogRead(SEN_RF);

    return IRReading { leftSide, leftAngled, leftForward, rightSide, rightAngled, rightForward };
}

IRReading MouseIR::read_all_calibrated()
{
    const IRReading before = read_all_ir();

    enable_ir(SensorDirection::Side);
    const unsigned leftSide = analogRead(SEN_LS);
    const unsigned rightSide = analogRead(SEN_RS);
    disable_ir(SensorDirection::Side);

    enable_ir(SensorDirection::Angle);
    const unsigned leftAngled = analogRead(SEN_LA);
    const unsigned rightAngled = analogRead(SEN_RA);
    disable_ir(SensorDirection::Angle);

    enable_ir(SensorDirection::Forward);
    const unsigned leftForward = analogRead(SEN_LF);
    const unsigned rightForward = analogRead(SEN_RF);
    disable_ir(SensorDirection::Forward);

    const IRReading after = {leftSide, leftAngled, leftForward, rightSide, rightAngled, rightForward};

    return IRReading::difference(before, after);
}

void MouseIR::disable_ir(const SensorDirection direction)
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
