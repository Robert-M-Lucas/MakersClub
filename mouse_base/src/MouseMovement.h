//
// Created by rober on 22/01/2024.
//

#ifndef MOUSEMOVEMENT_H
#define MOUSEMOVEMENT_H

struct Overshoot {
    int left;
    int right;
};

constexpr unsigned MOVEMENT_SPEED = 100;

class MouseMovement {
    bool movementInProgress = false;

    int leftStepsRemaining = 0;
    int rightStepsRemaining = 0;

    int motorDirectionLeft = 0;
    int motorDirectionRight = 0;

    bool prevLeftReading = false;
    bool prevRightReading = false;
public:
    static void runMotors(int left, int right);

    static Overshoot moveStepsBlocking(int stepsLeft, int stepsRight);

    void moveStepsNonBlocking(int stepsLeft, int stepsRight);

    bool updateMovement();
};


#endif //MOUSEMOVEMENT_H
