//
// Created by rober on 22/01/2024.
//

#include "MouseMovement.h"

#include <Arduino.h>
#include "Pins.h"

void MouseMovement::runMotors(int left, const int right) {

    left *= -1;

    if (right == 0) {
        digitalWrite(MOTOR_A1, 0);
        digitalWrite(MOTOR_A2, 0);
    }
    else if (right > 0) {
        digitalWrite(MOTOR_A1, 0);
        analogWrite(MOTOR_A2, right);
    }
    else {
        digitalWrite(MOTOR_A1, -right);
        analogWrite(MOTOR_A2, 0);
    }
    if (left == 0) {
        digitalWrite(MOTOR_B1, 0);
        digitalWrite(MOTOR_B2, 0);
    }
    else if (left > 0) {
        digitalWrite(MOTOR_B1, 0);
        analogWrite(MOTOR_B2, left);
    }
    else {
        digitalWrite(MOTOR_B1, -left);
        analogWrite(MOTOR_B2, 0);
    }
}

Overshoot MouseMovement::moveStepsBlocking(const int stepsLeft, const int stepsRight, const int stepSize = 0) {
    int left_remaining = abs(stepsLeft);
    int right_remaining = abs(stepsRight);

    const int motor_dir_left = stepsLeft > 0 ? 1 : -1;
    const int motor_dir_right = stepsRight > 0 ? 1 : -1;

    bool prev_left_reading = digitalRead(MAG_ENCA);
    bool prev_right_reading = digitalRead(MAG_ENCB);

    while (true) {
        const bool left_reading = digitalRead(MAG_ENCA);
        const bool right_reading = digitalRead(MAG_ENCB);

        // Every change in magnetism is 1 step
        if (left_reading != prev_left_reading) {
            left_remaining -= 1;
        }
        if (right_reading != prev_right_reading) {
            right_remaining -= 1;
        }
        prev_left_reading = left_reading;
        prev_right_reading = right_reading;

        if (left_remaining <= 0 && right_remaining <= 0) { break; }

        runMotors(
            left_remaining > 0 &&
                (stepSize == 0 || right_remaining - left_remaining < stepSize) ?
                motor_dir_left * static_cast<int>(MOVEMENT_SPEED) : 0,
            right_remaining > 0 &&
                (stepSize == 0 || left_remaining - right_remaining < stepSize) ?
                motor_dir_right * static_cast<int>(MOVEMENT_SPEED) : 0
        );
    }

    runMotors(0, 0);

    return Overshoot { left_remaining, right_remaining };
}

void MouseMovement::moveStepsNonBlocking(const int stepsLeft, const int stepsRight, const int stepSize = 0) {
    movementInProgress = true;

    leftStepsRemaining = stepsLeft;
    rightStepsRemaining = stepsRight;

    motorDirectionLeft = stepsLeft > 0 ? 1 : -1;
    motorDirectionRight = stepsRight > 0 ? 1 : -1;

    prevLeftReading = digitalRead(MAG_ENCA);
    prevRightReading = digitalRead(MAG_ENCB);

    MouseMovement::stepSize = stepSize;
}

bool MouseMovement::updateMovement() {
    if (!movementInProgress) { return false; }

    const bool leftReading = digitalRead(MAG_ENCA);
    const bool rightReading = digitalRead(MAG_ENCB);

    // Every change in magnetism is 1 step
    if (leftReading != prevLeftReading) {
        leftStepsRemaining -= motorDirectionLeft;
    }
    if (rightReading != prevRightReading) {
        rightStepsRemaining -= motorDirectionRight;
    }
    prevLeftReading = leftReading;
    prevRightReading = rightReading;

    if (leftStepsRemaining * motorDirectionLeft <= 0 && rightStepsRemaining * motorDirectionRight <= 0) {
        movementInProgress = false;
        runMotors(0, 0);
        return false;
    }

    runMotors(
        rightStepsRemaining * motorDirectionRight > 0 &&
        (stepSize == 0 || leftStepsRemaining - rightStepsRemaining < stepSize) ?
            motorDirectionRight * static_cast<int>(MOVEMENT_SPEED) : 0,
        leftStepsRemaining * motorDirectionLeft > 0 &&
        (stepSize == 0 || rightStepsRemaining - leftStepsRemaining < stepSize) ?
            motorDirectionLeft * static_cast<int>(MOVEMENT_SPEED) : 0
    );

    return true;
}