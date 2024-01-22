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

    /// Returns the increase between the before reading and the after reading
    static IRReading difference(const IRReading &before, const IRReading &after);

    /// Uses the IRCalibrationSet and the inverse-square law to map the readings to ones proportional to distance
    void calibrate(const IRCalibrationSet &calibrations);

    /// Prints the values in a human-readable way
    void serialPrettyPrintValues() const;

    /// Prints the values in a computer-readable way (specifically for the visualisation software)
    void serialOutputValues() const;
};


namespace MouseIR {
    /// Enables IR emitters in a given direction
    void enableIr(SensorDirection direction);
    /// Disables IR emitters in a given direction
    void disableIr(SensorDirection direction);

    /// Continuosly outputs current IR readings to serial until calibration parameters are returned through serial
    /// (specifically from the visualisation software)
    IRCalibrationSet getIrCalibrationsBlocking();

    /// Gets the raw IR readings from all sensors
    IRReading readAllIrRaw();
    /// Gets IR readings from all sensors adjusted for background levels
    IRReading readAllIrRelative();
    /// Gets IR readings from all sensors adjusted for background levels, min and max values, and maps them to
    /// readings proportional to distance using the inverse-square law
    IRReading readAllIrCalibrated(const IRCalibrationSet &calibrations);
};

#endif
