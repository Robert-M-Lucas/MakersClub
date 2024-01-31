//
// Created by rober on 29/01/2024.
//

#include "MazeInterface.h"

#include <MouseMovement.h>

void MazeInterface::MoveForward() {
    MouseMovement::moveStepsBlocking(FORWARD_STEPS, FORWARD_STEPS, FORWARD_SPEED);
}

void MazeInterface::TurnLeft() {
    MouseMovement::moveStepsBlocking(-TURN_STEPS, TURN_STEPS, TURN_SPEED);
}

void MazeInterface::TurnRight() {
    MouseMovement::moveStepsBlocking(TURN_STEPS, -TURN_STEPS, TURN_SPEED);
}

OpenPaths MazeInterface::GetOpenPaths() {
    return {};
}
