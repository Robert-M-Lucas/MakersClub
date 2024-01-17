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
    unsigned leftSide;
    unsigned leftAngled;
    unsigned leftForward;

    unsigned rightSide;
    unsigned rightAngled;
    unsigned rightForward;

    static IRReading difference(const IRReading &before, const IRReading &after);

    void serialPrettyPrintValues() const;

    void serialOutputValues() const;
};


namespace MouseIR {
    void enable_ir(SensorDirection direction);
    void disable_ir(SensorDirection direction);

    IRReading read_all_ir();
    IRReading read_all_calibrated();
};

#endif
