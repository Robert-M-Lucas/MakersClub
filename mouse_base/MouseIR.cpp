#include "MouseIR.h"
#include "Pins.h"
#include <FastLED.h>

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