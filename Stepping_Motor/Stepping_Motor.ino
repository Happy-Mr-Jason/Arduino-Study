#include <Stepper.h>
const int stepPerRevolution = 200;
Stepper stepping_motor(stepPerRevolution,11,10,9,8);

void setup()
{
    stepping_motor.setSpeed(20);
    Serial.begin(9600);
}

void loop()
{
    stepping_motor.step(100);
    delay(5000);
    stepping_motor.step(-100);
    delay(5000);
}