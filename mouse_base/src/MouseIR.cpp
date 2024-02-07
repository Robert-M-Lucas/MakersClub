#include <Arduino.h>
#include <FastLED.h>

#include "MouseIR.h"
#include "Pins.h"

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
    Serial.print(rightSide);
    Serial.print(";");
    Serial.print(rightAngled);
    Serial.print(";");
    Serial.print(rightForward);
    Serial.println();
}

IRReading IRReading::difference(const IRReading &before, const IRReading &after) {
    return IRReading {
        before.leftSide > after.leftSide ? 0 : after.leftSide - before.leftSide,
        before.leftAngled > after.leftAngled ? 0 : after.leftAngled - before.leftAngled,
        before.leftForward > after.leftForward ? 0 : after.leftForward - before.leftForward,
        before.rightSide > after.rightSide ? 0 : after.rightSide - before.rightSide,
        before.rightAngled > after.rightAngled ? 0 : after.rightAngled - before.rightAngled,
        before.rightForward > after.rightForward ? 0 : after.rightForward - before.rightForward,
    };
}

unsigned apply_calibration(const unsigned before, const IRCalibration calibration) {
    // Scale to floor and ceiling
    float scaled = max(0.0, static_cast<float>(before) - static_cast<float>(calibration.floor)) / (calibration.ceiling - calibration.floor) * 1023.0;
    // Get inverse square (inverse square law)
    constexpr float inv_sqr_min = sqrt(1.0f / 1023.0f);
    constexpr float inv_sqr_max = 1;
    scaled = max(1.0f, scaled);
    const float inv_sqr = sqrt(1.0f / scaled);
    // Re-scale to range - 0 -> 1023
    return static_cast<unsigned>((inv_sqr - inv_sqr_min) / (inv_sqr_max - inv_sqr_min) * 1023.0);
}

void IRReading::calibrate(const IRCalibrationSet &calibrations) {
    leftSide = apply_calibration(leftSide, calibrations.leftSide);
    leftAngled = apply_calibration(leftAngled, calibrations.leftAngled);
    leftForward = apply_calibration(leftForward, calibrations.leftForward);

    rightSide = apply_calibration(rightSide, calibrations.rightSide);
    rightAngled = apply_calibration(rightAngled, calibrations.rightAngled);
    rightForward = apply_calibration(rightForward, calibrations.rightForward);
}

void MouseIR::enableIr(const SensorDirection direction) {
    // Forward
    if (direction == SensorDirection::Forward) {
        PORTB |= 1<<0;
    }
    // Side
    else if (direction == SensorDirection::Side) {
        digitalWrite(IREMIT_SIDE, HIGH);
//        PORTC |= 1<<7;
    }
    // Angle
    else if (direction == SensorDirection::Angle) {
        digitalWrite(IREMIT_ANGLE, HIGH);
//        PORTB |= 1<<7;
    }
}


void MouseIR::disableIr(const SensorDirection direction) {
    if (direction == SensorDirection::Forward) {
        PORTB &= ~(1<<0);
    }
    else if (direction == SensorDirection::Side) {
        digitalWrite(IREMIT_SIDE, LOW);
//        PORTC &= ~(1<<7);
    }
    else if (direction == SensorDirection::Angle) {
        digitalWrite(IREMIT_ANGLE, LOW);
//        PORTB &= ~(1<<7);
    }
}

IRCalibrationSet MouseIR::getIrCalibrationsBlocking() {
    IRCalibrationSet calibrations = {};

    while (true) {
        if (Serial.available()) { // Receiving calibration
            unsigned i = 0;
            unsigned value = 0;
            char temp[] = {'x', '\0'};

            while (true) {
                const int read = Serial.read();
                if (read == -1) {
                    Serial.println("Waiting for data");
                    continue;
                }
                const char c = static_cast<char>(read);

                if (c == ';') { // Number completed
                    const bool is_floor = i % 2 == 0; // floor;ceiling;floor;ceiling; etc.
                    const unsigned sensor_index = i/2; // [0].f;[0].c;[1].f;[1].c etc.
                    IRCalibration *ir_calibration;

                    switch (sensor_index) {
                        case 0: ir_calibration = &calibrations.leftSide; break;
                        case 1: ir_calibration = &calibrations.leftAngled; break;
                        case 2: ir_calibration = &calibrations.leftForward; break;
                        case 3: ir_calibration = &calibrations.rightSide; break;
                        case 4: ir_calibration = &calibrations.rightAngled; break;
                        case 5: ir_calibration = &calibrations.rightForward; break;
                        default: ir_calibration = &calibrations.leftSide; break;
                    }

                    if (is_floor) {
                        ir_calibration->floor = value;
                    }
                    else {
                        ir_calibration->ceiling = value;
                    }

                    i++;
                    if (i == 12) { // All values sent
                        return calibrations;
                    }
                    value = 0;
                }

                temp[0] = c; // Convert char to string
                value *= 10; // Shift one place value
                value += atoi(temp); // Convert string to int
            }
        }

        // Read IR sensors
        const IRReading sensors = readAllIrRelative();
        // Output in code-readable way
        sensors.serialOutputValues();
        // Don't output faster than recipient can parse
        delay(50);
    }
}

IRReading MouseIR::readAllIrRaw() {
    const unsigned left_side = analogRead(SEN_LS);
    const unsigned left_angled = analogRead(SEN_LA);
    const unsigned left_forward = analogRead(SEN_LF);

    const unsigned right_side = analogRead(SEN_RS);
    const unsigned right_angled = analogRead(SEN_RA);
    const unsigned right_forward = analogRead(SEN_RF);

    return IRReading {
        left_side,
        left_angled,
        left_forward,
        right_side,
        right_angled,
        right_forward
    };
}

IRReading MouseIR::readAllIrRelative() {
    const IRReading before = readAllIrRaw();

    enableIr(SensorDirection::Side);
    const unsigned left_side = analogRead(SEN_LS);
    const unsigned right_side = analogRead(SEN_RS);
    disableIr(SensorDirection::Side);

    enableIr(SensorDirection::Angle);
    const unsigned left_angled = analogRead(SEN_LA);
    const unsigned right_angled = analogRead(SEN_RA);
    disableIr(SensorDirection::Angle);

    enableIr(SensorDirection::Forward);
    const unsigned left_forward = analogRead(SEN_LF);
    const unsigned right_forward = analogRead(SEN_RF);
    disableIr(SensorDirection::Forward);

    const IRReading after = {left_side,
        left_angled,
        left_forward,
        right_side,
        right_angled,
        right_forward
    };

    return IRReading::difference(before, after);
}

IRReading MouseIR::readAllIrCalibrated(const IRCalibrationSet &calibrations) {
    IRReading reading = readAllIrRelative();
    reading.calibrate(calibrations);
    return reading;
}