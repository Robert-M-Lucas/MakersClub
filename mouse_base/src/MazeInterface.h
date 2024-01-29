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

class MazeInterface {
public:
    void MoveForward();

    void TurnLeft();

    void TurnRight();

    OpenPaths GetOpenPaths();
};



#endif //MAZEINTERFACE_H
