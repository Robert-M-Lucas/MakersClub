#include <FastLED.h>

#include "Pins.h"
#include "Mouse.h"
#include "MouseIR.h"

CRGB addressable_led[1];

Mouse mouse;

void setup() {
    // configure the addressable led
    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(addressable_led, 1);

    Serial.begin(9600);
}

IRCalibration calibrations[6];

bool calibrated = false;

void loop()
{
    if (!calibrated) {
        if (Serial.available()) {
            int i = 0;
            unsigned value = 0;
            char temp[] = {'x', '\0'};
            while (true) {
                const int read = Serial.read();

                if (value == -1) {
                    Serial.println("Waiting for data");
                    continue;
                }

                const char c = static_cast<char>(read);

                if (c == ';') {
                    if (i % 2 == 0) {
                        calibrations[i/2].floor = value;
                    }
                    else {
                        calibrations[i/2].ceiling = value;
                    }
                    i++;
                    value = 0;
                    if (i == 12) {
                        calibrated = true;
                        Serial.println("Calibration complete");
                        return;
                    }
                }

                temp[0] = c;
                value *= 10;
                value += atoi(temp);
            }
        }

        const IRReading sensors = MouseIR::read_all_calibrated();
        // Output in code-readable way
        sensors.serialOutputValues();
        delay(50);
        return;
    }

    IRReading sensors = MouseIR::read_all_calibrated();
    Serial.println("Sensors before calibration:");
    sensors.serialPrettyPrintValues();
    sensors.calibrate(calibrations);
    Serial.println("After:");
    sensors.serialPrettyPrintValues();
}