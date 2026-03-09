#include <Arduino.h>
#include "Worker.h"
#include "Servo.h"


#define WorkingPin 12
#define ServoPin 3

#define MotorX1StepPin 37
#define MotorX1DirPin 35
#define MotorX1EnablePin 39

#define MotorX2StepPin 43
#define MotorX2DirPin 41
#define MotorX2EnablePin 45

#define MotorYStepPin 49
#define MotorYDirPin 47
#define MotorYEnablePin 51

#define LineSensorPin A13
#define LimitSwitch 2

#define downButton A11//когда 0, нажата
#define UpButton A12

Worker worker(ServoPin, MotorX1StepPin, MotorX1DirPin,MotorX1EnablePin,
   MotorX2StepPin, MotorX2DirPin,MotorX2EnablePin,
    MotorYStepPin, MotorYDirPin,MotorYEnablePin, LineSensorPin, LimitSwitch);

void setup() {
  Serial.begin(115200);
  pinMode(WorkingPin, OUTPUT);  
  pinMode(downButton, INPUT_PULLUP);
  pinMode(UpButton, INPUT_PULLUP);
  // pinMode(MotorX2DirPin, OUTPUT);
  // pinMode(MotorX2StepPin, OUTPUT);
  // pinMode(MotorX2EnablePin, OUTPUT);
  // pinMode(MotorX1DirPin, OUTPUT);
  // pinMode(MotorX1StepPin, OUTPUT);
  // pinMode(MotorX1EnablePin, OUTPUT);
  
  // pinMode(MotorYDirPin, OUTPUT);
  // pinMode(MotorYStepPin, OUTPUT);
  // pinMode(MotorYEnablePin, OUTPUT);

  // digitalWrite(MotorX1EnablePin, HIGH);
  // digitalWrite(MotorX2EnablePin, HIGH);
  // digitalWrite(MotorYEnablePin, HIGH);
  // delay(1000);  
  // digitalWrite(MotorX2DirPin,LOW);
  // digitalWrite(MotorX1DirPin,LOW);
  // delay(250);
  worker.Pen.attach(ServoPin);
  // worker.G28();
  // worker.M0();
  // worker.a();
  
}

void loop() {
  // worker.Work("G1 X100 Y100");
  if(digitalRead(downButton) == 0){
    worker.Down(); 
  }
  else if(digitalRead(UpButton) == 0){
    worker.Up();
  }
  digitalWrite(WorkingPin, HIGH);
  }
