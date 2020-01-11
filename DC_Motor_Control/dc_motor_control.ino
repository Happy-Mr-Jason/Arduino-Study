#define SERVO 11
#define MOTOR_CW 13
#define MOTOR_CCW 12

void setup()
{
  pinMode(SERVO, OUTPUT);
  pinMode(MOTOR_CW, OUTPUT);
  pinMode(MOTOR_CCW, OUTPUT);
}

void loop()
{
  digitalWrite(MOTOR_CW, LOW);
  digitalWrite(MOTOR_CCW, HIGH);
  analogWrite(SERVO,255);
  delay(1000);
  digitalWrite(MOTOR_CCW, LOW);
  digitalWrite(MOTOR_CW, HIGH);
  analogWrite(SERVO,0);
  delay(1000);
  
}