#ifndef MOUSEIR_H
#define MOUSEIR_H

enum SensorDirection {
    Forward = 1,
    Side = 2,
    Angle = 3
};

struct IRCalibration {
    unsigned floor;
    unsigned ceiling;
};

struct IRCalibrationSet {
    IRCalibration leftSide;
    IRCalibration leftAngled;
    IRCalibration leftForward;

    IRCalibration rightSide;
    IRCalibration rightAngled;
    IRCalibration rightForward;
};

class IRReading {
public:
    unsigned leftSide;
    unsigned leftAngled;
    unsigned leftForward;

    unsigned rightSide;
    unsigned rightAngled;
    unsigned rightForward;

    static IRReading difference(const IRReading &before, const IRReading &after);

    void calibrate(const IRCalibrationSet &calibrations);

    void serialPrettyPrintValues() const;

    void serialOutputValues() const;
};


namespace MouseIR {
    void enableIr(SensorDirection direction);
    void disableIr(SensorDirection direction);

    IRCalibrationSet getIrCalibrationsBlocking();

    IRReading readAllIrRaw();
    IRReading readAllIrRelative();
    IRReading readAllIrCalibrated(const IRCalibrationSet &calibrations);
};

#endif
