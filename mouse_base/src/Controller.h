#ifndef CONTROLLER_H
#define CONTROLLER_H

struct Steps {
    int left;
    int right;
    int lengthT;
}

class Controller {
private:
    int[][] mouseMap;
    Steps[] steps;
public:
    Controller(void);
    Steps[] getBacktrack(void);
    void mapPath(int lengthT, int left, int right);
    int turnMotor(int left, int right);
};

#endif
