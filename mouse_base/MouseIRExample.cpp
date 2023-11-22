#include "MouseIR.h"


void loop() {
    IRReading sensors = MouseIR::read_all_callibrated();

    sensors.leftSide;
    sensors.rightSide;

    sensors.leftAngled;
    sensors.rightAngled;

    sensors.leftForward;
    sensors.rightForward;
}
