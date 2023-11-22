#ifndef MouseIR_h
#define MouseIR_h

enum SensorDirection {
    Forward = 1,
    Side = 2,
    Angle = 3
};

class IRReading
{
public:
    unsigned left;
    unsigned leftAngled;
    unsigned leftForward;

    unsigned right;
    unsigned rightAngled;
    unsigned rightForward;

    static IRReading difference(IRReading before, IRReading after);
};


namespace MouseIR {
    void enable_ir(SensorDirection direction);
    void disable_ir(SensorDirection direction);

    IRReading read_all_ir();
    IRReading read_all_callibrated();
};

#endif
