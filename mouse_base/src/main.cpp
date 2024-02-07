#include <FastLED.h>

#include "Pins.h"
#include "Mouse.h"
#include "MouseIR.h"
#include "MouseMovement.h"
#include "MazeInterface.h"

// TODO: Is MSLEEP_PIN important
// TODO: Fill in all inline documentation in Pins.h
/* TODO:
 * What does Mouse::initialiseAllSensors() actually do to the IR emmitters and is the current IR code correctly
 * enabling and disabling emmitters?
 */
// TODO: Test calibration step
// TODO: Test movement with hall-effect sensors
// TODO: What does Controller.h do?
// TODO: Fill out MazeInterface
// TODO: Make MouseMovement compensate for overshoot

CRGB addressable_led[1];

IRCalibrationSet ir_calibrations;

void setup() {
    Serial.begin(9600);

    Serial.println("Initializing");

    Mouse::initialiseAllIO();

//    digitalWrite(IREMIT_ANGLE, HIGH);
//    digitalWrite(IREMIT_FORWARD, HIGH);
//    digitalWrite(IREMIT_SIDE, HIGH);

//    while (true) {
//        MouseIR::enableIr(SensorDirection::Forward);
//        MouseIR::enableIr(SensorDirection::Angle);
//        MouseIR::enableIr(SensorDirection::Side);
//        delay(500);
//        MouseIR::disableIr(SensorDirection::Forward);
//        MouseIR::disableIr(SensorDirection::Angle);
//        MouseIR::disableIr(SensorDirection::Side);
//        delay(500);
//    }
//    while (true) {
//        MouseIR::readAllIrRelative().serialPrettyPrintValues();
//    }

    // configure the addressable led
    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(addressable_led, 1);
    addressable_led[0].setRGB(255, 0, 0);

    Serial.println("Starting calibration - use calibration software to complete this step");
    ir_calibrations = MouseIR::getIrCalibrationsBlocking();
    Serial.println("Calibration complete");

    addressable_led[0].setRGB(0, 255, 0);

    // delay(3000);
    // for (int i = 0; i < 3; i++) { MazeInterface::MoveForward(); delay(500); }
    // MazeInterface::TurnRight(); delay(500);
    // for (int i = 0; i < 3; i++) { MazeInterface::MoveForward(); delay(500); }
    // MazeInterface::TurnRight(); delay(500);
    // for (int i = 0; i < 3; i++) { MazeInterface::MoveForward(); delay(500); }
    // MazeInterface::TurnRight(); delay(500);
    // for (int i = 0; i < 2; i++) { MazeInterface::MoveForward(); delay(500); }
    // MazeInterface::TurnRight(); delay(500);
    // for (int i = 0; i < 2; i++) { MazeInterface::MoveForward(); delay(500); }
}

void loop() {
    // IRReading sensors = MouseIR::readAllIrRelative();
    // Serial.println("Sensors before calibration:");
    // sensors.serialPrettyPrintValues();
    // sensors.calibrate(ir_calibrations);
    // Serial.println("After:");
    // sensors.serialPrettyPrintValues();
}