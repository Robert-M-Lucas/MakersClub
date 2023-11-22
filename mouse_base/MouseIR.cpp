#include "MouseIR.h"
#include "Pins.h"
#include <FastLED.h>

unsigned sub_or_zero(unsigned a, unsigned b) {
    if (a > b) {
        return 0;
    }
    return b - a;
}

IRReading IRReading::difference(IRReading before, IRReading after)
{
    return IRReading {
        sub_or_zero(before.left, after.left),
        sub_or_zero(before.leftAngled, after.leftAngled),
        sub_or_zero(before.leftForward, after.leftForward),

        sub_or_zero(before.right, after.right),
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
    unsigned left = analogRead(SEN_LS);
    unsigned leftAngled = analogRead(SEN_LA);
    unsigned leftForward = analogRead(SEN_LF);

    unsigned right = analogRead(SEN_RS);
    unsigned rightAngled = analogRead(SEN_RA);
    unsigned rightForward = analogRead(SEN_RF);

    return IRReading { left, leftAngled, leftForward, right, rightAngled, rightForward };
}

IRReading MouseIR::read_all_callibrated()
{
    IRReading before = read_all_ir();

    enable_ir(SensorDirection::Side);
    unsigned left = analogRead(SEN_LS);
    unsigned right = analogRead(SEN_RS);
    disable_ir(SensorDirection::Side);

    enable_ir(SensorDirection::Angle);
    unsigned leftAngled = analogRead(SEN_LA);
    unsigned rightAngled = analogRead(SEN_RA);
    disable_ir(SensorDirection::Angle);

    enable_ir(SensorDirection::Forward);
    unsigned leftForward = analogRead(SEN_LF);
    unsigned rightForward = analogRead(SEN_RF);
    disable_ir(SensorDirection::Forward);

    IRReading after = {left, leftAngled, leftForward, right, rightAngled, rightForward};

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
