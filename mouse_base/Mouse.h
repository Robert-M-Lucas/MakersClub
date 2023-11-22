#ifndef Mouse_h
#define Mouse_h

enum SensorDirection: uint8_t {
    Forward = 1,
    Side = 2,
    Angle = 3
}

class Mouse {
public:
    Mouse(void);
    void run_motors(int left, int right);
};
#endif
