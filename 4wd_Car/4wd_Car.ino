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

/*
* Bluetooth Communication
*/
//Bluetooth Serail
SoftwareSerial BTSerial(AD_RX_BT_TX, AD_TX_BT_RX);
const byte RCV_HEADER = 0x05;
const byte SND_HEADER = 0x06;
const byte TAIL_CR = 0x0D;
const byte TAIL_LF = 0x0A;
byte data_length = 0;
uint8_t buffer8[8];

/*
* ServoMotor
*/
//ServoMotor Nutral Position
const int S_NEUTRAL = 90;
//ServoMotors angle increase
const int S_INC = 1;
//ServoMotors angle decrease
const int S_DEC = 2;
//ServoMotor struct
struct ServoM
{
    Servo servo;
    int pinnum;
    //Servo Rotation Limits
    int limit_p;
    int limit_n;
    //Position Finished flag
    boolean positioning_Finished;
    void (*position)(int);
} mServo_B, mServo_T;
//Operation Mode (0 : Manual, 1 : Auto)
int operation_Mode = 0;
//Distans variables (Unit : cm)
int distance_F, distance_R, distance_L;

void setup()
{
    //Serial Init
    Serial.begin(9600);
    //Bluetooth Init
    BTSerial.begin(9600);
    //Boad IO Init
    board_io_Init();
    //Servo Motors
    servo_Motor_Init();
    //Car Init Stop
    stopCar();
}

void loop()
{
    //Bluetooth Communication
    bluetooth_Comms();
    //Mesurement of Distance
    distance_Front();
    distance_Left();
    distance_Right();
    //Motor Controll
    // analogWrite(L_SPEED, 255);
    // analogWrite(R_SPEED, 255);
    // turnCarLeft();
    // stopCar();
    // turnCarRight();
    // stopCar();

    // monitor_Serial();
}

void board_io_Init()
{
    //Ultrasonic Sensors Pins
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
}

//Bluetooth Communication Reading Buffer
void bluetooth_Comms()
{
    //Protocol - Receive
    //byte[0] : Start = ENQ
    //byte[1] : Address = register or id number
    //byte[2] : Datalength
    //byte[3:Datalength + 3] : Data
    //byte[Datalength + 4] : CR(0x0D) /r
    //byte[Datalength + 5] : LF(0x0A) /n
    //////////////////////////TODO
    if (BTSerial.available() > 0)
    {
        byte btread = BTSerial.read();
        Serial.print("Head : ");
        Serial.println(btread, DEC);
        byte tail_CR = 0;
        byte tail_LF = 0;
        if (btread == RCV_HEADER)
        {
            data_length = BTSerial.read();
            Serial.print("Data length : ");
            Serial.print(data_length, DEC);
            Serial.println();
            BTSerial.readBytes(buffer8, data_length);
            Serial.print("Data : ");
            Serial.print(buffer8[0], HEX);
            Serial.print(" ");
            Serial.print(buffer8[1], HEX);
            Serial.print(" ");
            Serial.print(buffer8[2], DEC);
            Serial.print(" ");
            Serial.print(buffer8[3], DEC);
            Serial.println();
            parsing_Buffer(buffer8);
        }
        else if (btread == TAIL_CR)
        {
            btread = BTSerial.read();
            if (btread == TAIL_LF)
            {
                Serial.println(F("EOF : CR LF"));
                bufferClear(buffer8);
            }
        }
        else
        {
            bufferClear(buffer8);
        }
    }
}

//Clear Buffer and Set Buffer_pos = 0
boolean bufferClear(byte b[])
{
    for (int i = 0; i < sizeof(b); i++)
    {
        b[i] = 0;
    }
    return true;
}

void buffer_copy(byte *src, byte *dst, int len)
{
    for (int i = 0; i < len; i++)
    {
        *dst++ = *src++;
    }
}

//Parsing Recived Data
void parsing_Buffer(byte cmdbuffer[])
{
    byte usebuffer[8];
    buffer_copy(cmdbuffer, usebuffer, 8);

    Serial.print("Recived Data : ");
    for (int i = 0; i < 8; i++)
    {
        Serial.print(usebuffer[i], HEX);
    }
    Serial.println();

    switch (usebuffer[0])
    {
    case 'C':
        btcmd_Car_Moving(usebuffer);
        break;
    case 'T':
        btcmd_Tilting_Moving(usebuffer);
        break;
    default:
        break;
    }
}

boolean btcmd_Car_Moving(byte cmdbuffer[])
{
    // servo_On();
    Serial.print("Car Moving Command : ");
    Serial.print((char)cmdbuffer[1]);
    int speedLeft = cmdbuffer[2];
    int speedRight = cmdbuffer[3];
    Serial.print(" ");
    Serial.print(speedLeft);
    Serial.print(" ");
    Serial.println(speedRight);

    analogWrite(L_SPEED, map(speedLeft, 0, 100, 0, 255));
    analogWrite(R_SPEED, map(speedRight, 0, 100, 0, 255));
    switch (cmdbuffer[1])
    {
    case 'F':
        runCarforward();
        break;
    case 'B':
        runCarBackward();
        break;
    }
    return true;
}

boolean btcmd_Tilting_Moving(byte cmdbuffer[])
{
    // servo_On();
    Serial.print("Tilting Moving Command : ");
    Serial.println((char)cmdbuffer[1]);
    switch (cmdbuffer[1])
    {
    case 'U':
        tilting_servo_Position(mServo_T, S_DEC);
        // tilting_servo_Position2(mServo_T, 60);
        break;
    case 'D':
        tilting_servo_Position(mServo_T, S_INC);
        // tilting_servo_Position2(mServo_T, 120);
        break;
    case 'L':
        tilting_servo_Position(mServo_B, S_INC);
        // tilting_servo_Position2(mServo_B, 120);
        break;
    case 'R':
        tilting_servo_Position(mServo_B, S_DEC);
        // tilting_servo_Position2(mServo_B, 60);
        break;
    case 'N':
        tilting_servo_Position2(mServo_B, S_NEUTRAL);
        tilting_servo_Position2(mServo_T, S_NEUTRAL);
        break;
    case 'T':
        break;
    }
    return true;
}

void servo_On()
{
    mServo_B.servo.attach(mServo_B.pinnum);
    mServo_T.servo.attach(mServo_T.pinnum);
}

void servo_Off()
{
    mServo_B.servo.detach();
    mServo_T.servo.detach();
}

//Servo Pin Setup
//Base Limits Setup
//Tilting Limits Setup
//Servo Motors Positioning Finish Flag
//Servo Motors Init Positioning (NEUTRAL = 90)
void servo_Motor_Init()
{
    //Servo Pin Setup
    mServo_B.pinnum = S_BASE;
    mServo_T.pinnum = S_TILT;
    mServo_B.servo.attach(mServo_B.pinnum);
    mServo_T.servo.attach(mServo_T.pinnum);
    //Base Limits Setup
    mServo_B.limit_p = 120;
    mServo_B.limit_n = 60;
    //Tilting Limits Setup
    mServo_T.limit_p = 120;
    mServo_T.limit_n = 60;
    //Servo Motors Positioning Finish Flag
    mServo_B.positioning_Finished = false;
    mServo_T.positioning_Finished = false;
    //Servo Motors Init Positioning (NEUTRAL = 90)
    mServo_B.servo.write(S_NEUTRAL);
    mServo_T.servo.write(S_NEUTRAL);
    mServo_B.servo.detach();
    mServo_T.servo.detach();
}

//Servomotor Positioning
//input = S_INC or S_DEC
boolean tilting_servo_Position(ServoM sv, int input)
{
    Serial.println("Run Tilting Servo 1...");
    sv.servo.attach(sv.pinnum);
    while (!sv.servo.attached())
        ;
    int cmd_position = sv.servo.read();
    if (input == S_INC)
    {
        cmd_position += 10;
        if (cmd_position > sv.limit_p)
        {
            cmd_position = sv.limit_p;
        }
    }
    else if (input == S_DEC)
    {
        cmd_position -= 10;
        if (cmd_position < sv.limit_n)
        {
            cmd_position = sv.limit_n;
        }
    }

    int current_position = sv.servo.read();
    if (current_position < cmd_position)
    {
        for (int i = current_position; i <= cmd_position; i++)
        {
            sv.servo.write(i);
        }
    }
    else if (current_position > cmd_position)
    {
        for (int i = current_position; i >= cmd_position; i--)
        {
            sv.servo.write(i);
        }
    }
    delay(500);
    sv.servo.detach();
    return true;
}

boolean tilting_servo_Position2(ServoM sv, int angle)
{
    Serial.println("Run Tilting Servo 2 ...");
    sv.servo.attach(sv.pinnum);
    while (!sv.servo.attached())
        ;
    int current_position = sv.servo.read();
    int cmd_position = angle;

    if (cmd_position > sv.limit_p)
    {
        cmd_position = sv.limit_p;
    }

    if (cmd_position < sv.limit_n)
    {
        cmd_position = sv.limit_n;
    }

    if (current_position < cmd_position)
    {
        for (int i = current_position; i <= cmd_position; i++)
        {
            sv.servo.write(i);
        }
    }
    else if (current_position > cmd_position)
    {
        for (int i = current_position; i >= cmd_position; i--)
        {
            sv.servo.write(i);
        }
    }
    delay(500);
    sv.servo.detach();
    return true;
}

//Serial Monitoring
void monitor_Serial()
{
    //Operation Mode
    monitor_Seperator_Tag("Operation Mode");
    Serial.println(operation_Mode == 1 ? String("Automatic mode") : String("Manual Mode"));
    //Distance Monitoring
    monitor_Seperator_Tag("Distance");
    Serial.print(String("Front : ") + distance_F + String("cm"));
    Serial.print(String(" / Left : ") + distance_L + String("cm"));
    Serial.println(String(" / Right : ") + distance_R + String("cm"));
    //Servo Motor Monitoring
    monitor_Seperator_Tag("Servo Motors");
    Serial.print(String("Base-Angle : ") + mServo_B.servo.read() + char(' '));
    Serial.println(String("Tilt-Angle : ") + mServo_T.servo.read());
}

//Serial Monitoring Seperator
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

void holdTurnCarRight()
{
    turnCarRight;
    returnCarLeft;
}

void holdTurnCarLeft()
{
    turnCarLeft;
    returnCarRight;
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