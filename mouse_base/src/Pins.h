#ifndef PINS_H
#define PINS_H

/// ???
#define LED_PIN 7 //PE6

/// ???
#define MSLEEP_PIN 12 // PD6
/// ???
#define MOTOR_A1 6 // PD7
/// ???
#define MOTOR_A2 9 // PB4
/// ???
#define MOTOR_B1 5 // PC6
/// ???
#define MOTOR_B2 10 // PCPB6

//#define IREMIT_FORWARD //RXLED //PB0
/// ???
#define IREMIT_SIDE 13 // PC7
/// ???
#define IREMIT_ANGLE 11 // PB7

/// Analog Left-Forward IR sensor
#define SEN_LF A5 // PF0
/// Analog Left-Angled IR sensor
#define SEN_LA A4 // PF1
/// Analog Left-Side IR sensor
#define SEN_LS A3 // PF4
/// Analog Right-Side IR sensor
#define SEN_RS A2 // PF5
/// Analog Right-Angled IR sensor
#define SEN_RA A1 // PF6
/// Analog Right-Forward IR sensor
#define SEN_RF A0 // PF7

/// Digital input pin for the left hall-effect sensor
#define MAG_ENCA 4 // PD4
/// Digital input pin for the right hall-effect sensor
#define MAG_ENCB 30 // PD5

#endif