//
// Created by rober on 29/01/2024.
//

#ifndef MAZEINTERFACE_H
#define MAZEINTERFACE_H

struct OpenPaths {
    bool left;
    bool front;
    bool right;
};

constexpr int FORWARD_STEPS = 26;
constexpr int FORWARD_SPEED = 200;
constexpr int TURN_STEPS = 9;
constexpr int TURN_SPEED = 50;

class MazeInterface {
public:
    static void MoveForward();

    static void TurnLeft();

    static void TurnRight();

    static OpenPaths GetOpenPaths();
};



#endif //MAZEINTERFACE_H
