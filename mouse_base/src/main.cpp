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

CRGB addressable_led[1];

IRCalibrationSet ir_calibrations;

void setup() {
    Serial.begin(9600);

    Serial.println("Initialising");

    Mouse::initialiseAllIO();

    // configure the addressable led
    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(addressable_led, 1);


    // Serial.println("Starting calibration - use calibration software to complete this step");
    // ir_calibrations = MouseIR::getIrCalibrationsBlocking();
    // Serial.println("Calibration complete");
    int fwd = 26;
    int trn = 6;

    delay(3000);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(trn, -trn, 5);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(trn, -trn, 5);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(trn, -trn, 5);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(trn, -trn, 5);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
    delay(500);
    MouseMovement::moveStepsBlocking(fwd, fwd, 3);
}

void loop() {
    // IRReading sensors = MouseIR::readAllIrRelative();
    // Serial.println("Sensors before calibration:");
    // sensors.serialPrettyPrintValues();
    // sensors.calibrate(ir_calibrations);
    // Serial.println("After:");
    // sensors.serialPrettyPrintValues();
}