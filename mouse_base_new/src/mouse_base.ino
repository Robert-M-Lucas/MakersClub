#include <FastLED.h>

#include "Pins.h"
#include "Mouse.h"
#include "MouseIR.h"

CRGB addressable_led[1];

Mouse mouse;

uint8_t a = MAG_ENCA;

void setup() { 
    // configure the addressable led
    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(addressable_led, 1);

    Serial.begin(9600);
    // pinMode(a, INPUT);

    pinMode(4, INPUT);
}

void test();

void loop()
{
    test();
}

int bias = 0;
int biasLimit = 100;
constexpr int constantBias = -20;

void test()
{
    Serial.println(digitalRead(4));
    return;

    IRReading sensors = MouseIR::read_all_calibrated();
    // Output in code-readable way
    sensors.serialOutputValues();
    delay(50);
    return;

    sensors.serialPrettyPrintValues();

    bias += ((int) (sensors.leftSide + sensors.leftAngled)) - ((int) (sensors.rightSide + sensors.rightAngled)) + bias + bias;
    bias /= 4;
    bias = constrain(bias, -biasLimit, biasLimit);

    Serial.print("Bias: ");
    Serial.println(bias);

    bias = 0;

    mouse.run_motors(100 + (bias / 3) + constantBias, 100 - (bias / 3) - constantBias);

    delay(100);
}
