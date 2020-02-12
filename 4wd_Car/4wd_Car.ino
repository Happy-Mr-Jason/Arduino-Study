#include <SoftwareSerial.h>
#include <Servo.h>

//Bluetooth Pins
#define AD_TX_BT_RX 12
#define AD_RX_BT_TX 13
//Ultrasonic Sensor Pins
#define UF_ECHO 2
#define UF_TRIG 3
#define UR_ECHO 4
#define UR_TRIG 5
#define UL_ECHO 6
#define UL_TRIG 7
//DC Motor controller(L298N) Pins
#define M_IN1 11
#define M_IN2 10
#define M_IN3 9
#define M_IN4 8
#define L_SPEED A2
#define R_SPEED A3
//Servo Motor Pins
#define S_BASE A0
#define S_TILT A1

//Bluetooth Communication
SoftwareSerial BTSerial(AD_RX_BT_TX, AD_TX_BT_RX);
//ServoMotors
Servo mServo_B, mServo_T;
//Base Rotation Limits
int mServo_B_limit_p = 110;
int mServo_B_limit_n = 70;
//Tilter Tilting Limits
int mServo_T_limit_p = 110;
int mServo_T_limit_n = 70;
//Operation Mode (0 : Manual, 1 : Auto)
int operation_Mode = 0;
//Distans variables (Unit : cm)
int distance_F, distance_R, distance_L;

void setup()
{
    Serial.begin(9600);
    BTSerial.begin(9600);
    //Ultrasonic Sensors
    pinMode(UF_TRIG, OUTPUT);
    pinMode(UF_ECHO, INPUT);
    pinMode(UR_TRIG, OUTPUT);
    pinMode(UR_ECHO, INPUT);
    pinMode(UL_TRIG, OUTPUT);
    pinMode(UL_ECHO, INPUT);
    //DC Motor Controller Pins
    pinMode(M_IN1, OUTPUT);
    pinMode(M_IN2, OUTPUT);
    pinMode(M_IN3, OUTPUT);
    pinMode(M_IN4, OUTPUT);
    pinMode(L_SPEED, OUTPUT);
    pinMode(R_SPEED, OUTPUT);
    //Servo Motors
    mServo_B.attach(S_BASE);
    mServo_T.attach(S_TILT);
    //Servo Motors Init Positioning (Center)
    mServo_B.write(90);
    mServo_T.write(90);
    //Car Init Stop
    stopCar();
}

void loop()
{
    //Bluetooth Communication
    bluetooth_Coms();
    //Mesurement of Distance
    distance_Front();
    distance_Left();
    distance_Right();

    //Motor Controll
    analogWrite(L_SPEED, 255);
    analogWrite(R_SPEED, 255);
    // turnCarLeft();
    // stopCar();
    // turnCarRight();
    // stopCar();

    monitor_Serial();
}

void bluetooth_Coms(){
    //TODO///////////////////////////////////
}

void monitor_Serial()
{
    //Operation Mode
    monitor_Seperator_Tag("Operation Mode");
    Serial.println(operation_Mode == 1? String("Automatic mode") : String("Manual Mode"));
    //Distance Monitoring
    monitor_Seperator_Tag("Distance");
    Serial.print(String("Front : ") + distance_F + String("cm"));
    Serial.print(String(" / Left : ") + distance_L + String("cm"));
    Serial.println(String(" / Right : ") + distance_R + String("cm"));
    //Servo Motor Monitoring
    monitor_Seperator_Tag("Servo Motors");
    Serial.print(String("Base-Angle : ") + mServo_B.read() + char(' '));
    Serial.println(String("Tilt-Angle : ") + mServo_T.read());
}

void monitor_Seperator_Tag(String monitor_name)
{
    Serial.print(char('[') + monitor_name + char(']'));
    for (int i = 0; i < 50 - monitor_name.length(); i++)
    {
        if (i == 49 - monitor_name.length())
        {
            Serial.println(String("="));
        }
        else
        {
            Serial.print(String("="));
        }
    }
}

void distance_Front()
{
    digitalWrite(UF_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(UF_TRIG, HIGH);
    delayMicroseconds(2);
    digitalWrite(UF_TRIG, LOW);
    distance_F = (int)(pulseIn(UF_ECHO, HIGH) / 58.2);
}

void distance_Right()
{
    digitalWrite(UR_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(UR_TRIG, HIGH);
    delayMicroseconds(2);
    digitalWrite(UR_TRIG, LOW);
    distance_R = (int)(pulseIn(UR_ECHO, HIGH) / 58.2);
}

void distance_Left()
{
    digitalWrite(UL_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(UL_TRIG, HIGH);
    delayMicroseconds(2);
    digitalWrite(UL_TRIG, LOW);
    distance_L = (int)(pulseIn(UL_ECHO, HIGH) / 58.2);
}

void stopCar()
{
    stopLeft();
    stopRight();
}

void runCarforward()
{
    runRightForward();
    runLeftForward();
}

void runCarBackward()
{
    runRightBackward();
    runLeftBackward();
}

void turnCarLeft()
{
    runRightForward();
}

void turnCarRight()
{
    runLeftForward();
}

void returnCarLeft()
{
    runRightBackward();
}

void returnCarRight()
{
    runLeftBackward();
}

void runLeftForward()
{
    digitalWrite(M_IN1, HIGH);
    digitalWrite(M_IN2, LOW);
    Serial.println("Left Forward");
}

void runLeftBackward()
{
    digitalWrite(M_IN1, LOW);
    digitalWrite(M_IN2, HIGH);
    Serial.println("Left Backward");
}

void stopLeft()
{
    digitalWrite(M_IN1, LOW);
    digitalWrite(M_IN2, LOW);
    Serial.println("Left Stop");
}

void runRightForward()
{
    digitalWrite(M_IN3, HIGH);
    digitalWrite(M_IN4, LOW);
    Serial.println("Right Forward");
}

void runRightBackward()
{
    digitalWrite(M_IN3, LOW);
    digitalWrite(M_IN4, HIGH);
    Serial.println("Right Backward");
}
void stopRight()
{
    digitalWrite(M_IN3, LOW);
    digitalWrite(M_IN4, LOW);
    Serial.println("Right Stop");
}