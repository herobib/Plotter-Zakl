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

#define downButton A11//когда 0, нажата
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
  // worker.G28();
  // delay(2000);
  // worker.a();
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
  else if(digitalRead(Button1) == 0){
    Serial.println("Button 1");
  }
  else if(digitalRead(Button2) == 0){
    Serial.println("Button 2");
  }
  else if(digitalRead(Button3) == 0){
    Serial.println("Button 3");
  }
  else if(digitalRead(Button4) == 0){
    Serial.println("Button 4");
  }
  else if(digitalRead(Button5) == 0){
    Serial.println("Button 5");
  }
  else if(digitalRead(Button6) == 0){
    Serial.println("Button 6");
  }
  else if(digitalRead(Button7) == 0){
    Serial.println("Button 7");
  }
  else if(digitalRead(Button8) == 0){
    Serial.println("Button 8");
  }
  else if(digitalRead(Button9) == 0){
    Serial.println("Button 9");
  }
  else if(digitalRead(Button10) == 0){
    Serial.println("Button 10");
  }
  digitalWrite(WorkingPin, HIGH);
  // Serial.println(analogRead(LineSensorPin));
  }
