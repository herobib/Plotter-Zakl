//последнюю версию кода можно найти в моем репозитории гитхаба
//https://github.com/herobib/Plotter-Zakl.git

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

#define downButton A11
#define UpButton A12

#define Button1 A10
#define Button2 A8
#define Button3 A7
#define Button4 A6
#define Button5 A5
#define Button6 A4
#define Button7 A3
#define Button8 A2
#define Button9 A1
#define Button10 A0


Worker worker(ServoPin, MotorX1StepPin, MotorX1DirPin,MotorX1EnablePin,
   MotorX2StepPin, MotorX2DirPin,MotorX2EnablePin,
    MotorYStepPin, MotorYDirPin,MotorYEnablePin, LineSensorPin, LimitSwitch);

void setup() {
  Serial.begin(115200);
  pinMode(WorkingPin, OUTPUT);  
  pinMode(downButton, INPUT_PULLUP);
  pinMode(UpButton, INPUT_PULLUP);
  
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(Button3, INPUT_PULLUP);
  pinMode(Button4, INPUT_PULLUP);
  pinMode(Button5, INPUT_PULLUP);
  pinMode(Button6, INPUT_PULLUP);
  pinMode(Button7, INPUT_PULLUP);
  pinMode(Button8, INPUT_PULLUP);
  pinMode(Button9, INPUT_PULLUP);
  pinMode(Button10, INPUT_PULLUP);

  worker.Pen.attach(ServoPin);
  worker.M1();
  // worker.CalibrationTestY();
  
}

void loop() {
  // worker.M1();
  // delay(1000);
  // worker.M0();
  // delay(1000);
  if(digitalRead(downButton) == 0){
    worker.Down(); 
  }
  else if(digitalRead(UpButton) == 0){
    worker.Up();  
  }
  else if(digitalRead(Button1) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 1");
    worker.G28();
    worker.GoTo(70,20);
    worker.G1(70,80);
    worker.G1(120,80);
    worker.G1(150,50);
    worker.G1(120,20);
    worker.G1(70,20);
    worker.GoTo(85,35);
    worker.G1(85,65);
    worker.G1(105,65);
    worker.G1(105,35);
    worker.G1(85,35);
    worker.G1(105,65);
    worker.GoTo(105,35);
    worker.G1(85,65);
    worker.BackToStart();
    digitalWrite(WorkingPin, LOW);
  }
  else if(digitalRead(Button2) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 2");
    worker.G28();
    worker.GoTo(10,80);
    worker.G1(10,-70);
    worker.G1(160,-70);
    worker.BackToStart();
    digitalWrite(WorkingPin, LOW);
  }
  else if(digitalRead(Button3) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 3");
    worker.G28();
    worker.GoTo(40,-60);
    worker.G2(40,-30,40,0);
    worker.G3(40,30,40,60);
    worker.BackToStart();
    digitalWrite(WorkingPin, LOW);
  }
  else if(digitalRead(Button4) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 4");
    digitalWrite(WorkingPin, LOW);
  }
  else if(digitalRead(Button5) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 5");
    digitalWrite(WorkingPin, LOW);
  }
  else if(digitalRead(Button6) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 6");
    digitalWrite(WorkingPin, LOW);
  }
  else if(digitalRead(Button7) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 7");
    digitalWrite(WorkingPin, LOW);
  }
  else if(digitalRead(Button8) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 8");
    digitalWrite(WorkingPin, LOW);
  }
  else if(digitalRead(Button9) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 9");
    digitalWrite(WorkingPin, LOW);
  }
  else if(digitalRead(Button10) == 0){
    digitalWrite(WorkingPin, HIGH);
    Serial.println("Button 10");
    digitalWrite(WorkingPin, LOW);
  }
  
  // Serial.println(analogRead(LineSensorPin));
  }
