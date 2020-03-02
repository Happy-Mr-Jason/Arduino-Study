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
Car Operation Mode
*/
int car_Mode = 0;
boolean car_stopped = true;
#define CAR_MODE_JOYSTIC 1
#define CAR_MODE_BUTTON 2
#define CAR_MODE_SENSOR 3
/*
Car Sensor Mode
*/
int car_sensorMode = 0;
#define CAR_SENSOR_MODE_STOP 0
#define CAR_SENSOR_MODE_START 1
boolean car_sensormodestarted = false;
#define SEL_DIRECTION_FRONT 1
#define SEL_DIRECTION_LEFT 2
#define SEL_DIRECTION_RIGHT 3
#define SEL_DIRECTION_BACK 4
int selectedDirection = 0;
//memory of prev Direction
byte prevDirection;
//Front blocked distance
int f_block_d = 10;
//Front Turnable free distance
int f_free_d = 15;
//Left-Right compare threshold distance
int lr_comp_th_d = 5;
int target_d = 0;

/*
Camera Tilting Operation Mode
*/
int tilting_Mode = 0;
#define TILTING_MODE_BUTTON 1
#define TILTING_MODE_TRACKING 2

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
/*
  loop interval timer millis();
*/
long loop_current_time;
long timer_1sec_start;
const long timer_1sec_interval = 1000;
boolean timer_1sec_on = false;

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
    loop_current_time = millis();
    if (loop_current_time > (timer_1sec_start + timer_1sec_interval))
    {
        timer_1sec_on = true;
        timer_1sec_start = loop_current_time;
    }
    else
    {
        timer_1sec_on = false;
    }
    //Bluetooth Communication
    bluetooth_Comms();
    //Mesurement of Distance
    distance_Front();
    distance_Left();
    distance_Right();
    //CAR Sensor Mode

    if (timer_1sec_on)
    {
        monitor_Serial();
        if (car_Mode == CAR_MODE_SENSOR)
        {
            if (car_sensorMode == CAR_SENSOR_MODE_START)
            {
                sensormodeStart();
            }
            else
            {

                sensormodeStop();
            }
        }
    }

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
    int mode = cmdbuffer[2];
    int sensormoderun = cmdbuffer[2];
    int speedLeft = cmdbuffer[2];
    int speedRight = cmdbuffer[3];
    Serial.print(" ");
    Serial.print(speedLeft);
    Serial.print(" ");
    Serial.println(speedRight);

    switch (cmdbuffer[1])
    {
    case 'F':
        carSpeedSelect(speedLeft, speedRight);
        runCarForward();
        break;
    case 'B':
        carSpeedSelect(speedLeft, speedRight);
        runCarBackward();
        break;
    case 'L':
        carSpeedSelect(speedLeft, speedRight);
        turnCarLeft();
        break;
    case 'R':
        carSpeedSelect(speedLeft, speedRight);
        turnCarRight();
        break;
    case 'S':
        stopCar();
        break;
    case 'A':
        carSensorMode(sensormoderun);
        break;
    case 'M':
        carModeChange(mode);
        break;
    }
    return true;
}

void carSpeedSelect(int leftspeed, int rightspeed)
{
    if (car_Mode == CAR_MODE_BUTTON)
    {
        analogWrite(L_SPEED, 255);
        analogWrite(R_SPEED, 255);
    }
    else
    {
        analogWrite(L_SPEED, map(leftspeed, 0, 100, 0, 255));
        analogWrite(R_SPEED, map(rightspeed, 0, 100, 0, 255));
    }
}

void carModeChange(int selectedMode)
{
    switch (selectedMode)
    {
    case CAR_MODE_JOYSTIC:
        car_Mode = CAR_MODE_JOYSTIC;
        break;
    case CAR_MODE_BUTTON:
        car_Mode = CAR_MODE_BUTTON;
        break;
    case CAR_MODE_SENSOR:
        car_Mode = CAR_MODE_SENSOR;
        break;
    }
}

void carSensorMode(int sensormoderun)
{
    switch (sensormoderun)
    {
    case 0:
        car_sensorMode = CAR_SENSOR_MODE_STOP;
        break;
    case 1:
        car_sensorMode = CAR_SENSOR_MODE_START;
        break;
    }
}

void sensormodeStart()
{
    if (car_sensormodestarted != true)
    {
        Serial.println("Sensor Mode Start!!");
    }
    car_sensormodestarted = true;
    car_stopped = false;

    selectedDirection = selectDirection();
    Serial.print("Selected Direction : ");

    switch (selectedDirection)
    {
    case SEL_DIRECTION_FRONT:
        Serial.println("Front");
        analogWrite(L_SPEED, 255);
        analogWrite(R_SPEED, 255);
        runCarForward();
        break;
    case SEL_DIRECTION_BACK:
        Serial.println("Back");
        analogWrite(L_SPEED, 130);
        analogWrite(R_SPEED, 130);
        runCarBackward();
        break;
    case SEL_DIRECTION_LEFT:
        Serial.println("Left");
        analogWrite(L_SPEED, 130);
        analogWrite(R_SPEED, 130);
        holdTurnCarLeft();
        break;
    case SEL_DIRECTION_RIGHT:
        Serial.println("Right");
        analogWrite(L_SPEED, 130);
        analogWrite(R_SPEED, 130);
        holdTurnCarRight();
        break;
    }

    Serial.print("Target Distance : ");
    Serial.println(target_d);
}

void sensormodeStop()
{
    if (car_sensormodestarted != false)
    {
        Serial.println("Sensor Mode Stop!!");
    }
    car_sensormodestarted = false;
    if (car_stopped != true)
    {
        stopCar();
    }
}

int selectDirection()
{
    memprevDirection(selectedDirection);

    if (prevDirection >= 5 && distance_F >= target_d)
    {
        prevDirection = 0;
        target_d = 0;
    }

    if (distance_F > f_block_d && !checkmemDirection(SEL_DIRECTION_FRONT) && prevDirection == 0)
    {
        return SEL_DIRECTION_FRONT;
    }
    else if (distance_L > f_block_d && distance_L > distance_R + lr_comp_th_d && distance_F > f_free_d)
    {
        if (target_d == 0)
        {
            target_d = distance_L;
        }
        return SEL_DIRECTION_LEFT;
    }
    else if (distance_R > f_block_d && distance_R > distance_L + lr_comp_th_d && distance_F > f_free_d)
    {
        if (target_d == 0)
        {
            target_d = distance_R;
        }
        return SEL_DIRECTION_RIGHT;
    }
    else if (!checkmemDirection(SEL_DIRECTION_BACK) && (distance_F < f_free_d || (distance_L < f_block_d && distance_R < f_block_d)))
    {

        return SEL_DIRECTION_BACK;
    }
}

void memprevDirection(int saveDirection)
{
    switch (saveDirection)
    {
    case SEL_DIRECTION_FRONT:
        prevDirection = prevDirection | 0x01;
        break;
    case SEL_DIRECTION_BACK:
        prevDirection = prevDirection | 0x02;
        break;
    case SEL_DIRECTION_LEFT:
        prevDirection = prevDirection | 0x04;
        break;
    case SEL_DIRECTION_RIGHT:
        prevDirection = prevDirection | 0x08;
        break;
    }
}

boolean checkmemDirection(int checkDirection)
{
    byte check;
    switch (checkDirection)
    {
    case SEL_DIRECTION_FRONT:
        check = prevDirection & 0x01;
        break;
    case SEL_DIRECTION_BACK:
        check = prevDirection & 0x02;
        break;
    case SEL_DIRECTION_LEFT:
        check = prevDirection & 0x04;
        break;
    case SEL_DIRECTION_RIGHT:
        check = prevDirection & 0x08;
        break;
    }
    Serial.print("Check Direction Memory : ");
    Serial.println(prevDirection);

    if (check > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

boolean btcmd_Tilting_Moving(byte cmdbuffer[])
{
    // servo_On();
    Serial.print("Tilting Moving Command : ");
    Serial.print((char)cmdbuffer[1]);
    int mode = cmdbuffer[2];
    Serial.print(" ");
    Serial.println(mode);
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
    case 'M':
        tiltingModeChange(mode);
        break;
    }
    return true;
}

void tiltingModeChange(int selectedMode)
{
    switch (selectedMode)
    {
    case TILTING_MODE_BUTTON:
        tilting_Mode = TILTING_MODE_BUTTON;
        break;
    case TILTING_MODE_TRACKING:
        tilting_Mode = TILTING_MODE_TRACKING;
        break;
    }
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
    car_stopped = true;
}

void runCarForward()
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
    turnCarRight();
    returnCarLeft();
}

void holdTurnCarLeft()
{
    turnCarLeft();
    returnCarRight();
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