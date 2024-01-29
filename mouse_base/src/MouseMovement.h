//
// Created by rober on 22/01/2024.
//

#ifndef MOUSEMOVEMENT_H
#define MOUSEMOVEMENT_H

struct Overshoot {
    int left;
    int right;
};

constexpr unsigned short MOVEMENT_SPEED = 100;

class MouseMovement {
    bool movementInProgress = false;

    int leftStepsRemaining = 0;
    int rightStepsRemaining = 0;

    int motorDirectionLeft = 0;
    int motorDirectionRight = 0;

    bool prevLeftReading = false;
    bool prevRightReading = false;

    int stepSize = 0;
public:
    /// Sets the left/right motors to run at the given values
    static void runMotors(int left, int right);

    /// Runs the motors until they have moved the given number of steps
    /// <returns>Returns the overshoot of the target steps</returns>
    static Overshoot moveStepsBlocking(int stepsLeft, int stepsRight, int stepSize);

    /// Starts a movement that will run the motors until they have moved the given number of steps
    ///
    /// Requires updateMovement to be called in a tight loop until the movement is complete
    void moveStepsNonBlocking(int stepsLeft, int stepsRight, int stepSize);

    /// Moves the mouse until the current movement is completed
    ///
    /// !WARNING! If not called frequently enough the step measurement will be innaccurate! Ensure that no blocking code
    /// is run in the loop that contains this method call
    ///
    /// <returns>false when the movement is completed</returns>
    bool updateMovement();
};


#endif //MOUSEMOVEMENT_H
