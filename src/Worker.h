#include <Arduino.h>
#include "Nema17.h"
#include "Servo.h"

class Worker {
    private:
    int LineSensorPin;
    int LimitSwitchPin;
    MotorX Mx; 
    MotorY My;
    int Xcoord;
    int Ycoord;
    int PenOutput=MMtoStepsX(12);   
    public:
    Servo Pen;
    Worker(int Servo_pin,int mx1_setp_pin, int mx1_dir_pin,int mx1_en_pin,
         int mx2_setp_pin, int mx2_dir_pin ,int mx2_en_pin,
        int my_setp_pin, int my_dir_pin, int my_en_pin, 
        int line_sensor_pin, int limit_switch_pin){
        Mx = MotorX(mx1_setp_pin, mx1_dir_pin,mx1_en_pin,mx2_setp_pin,mx2_dir_pin,mx2_en_pin);
        My = MotorY(my_setp_pin, my_dir_pin,my_en_pin);
        LineSensorPin = line_sensor_pin;
        LimitSwitchPin = limit_switch_pin;
        pinMode(Servo_pin, OUTPUT);
        pinMode(LineSensorPin, INPUT);
        pinMode(LimitSwitchPin, INPUT);
        Serial.begin(115200);
    }

    void Work(String gcode){
        for(int i=0;i<100;i++){
                Mx.GoOnStep(10000, true);
        }
    }

    int MMtoStepsY(float mm){
        return abs(int(5.04*mm - 6.1916)); // как-то обрабатываю
    }
    int MMtoStepsX(float mm){
        return int(mm*10); // как-то обрабатываю
    }

    void G28(){
        M1();
        while(digitalRead(LimitSwitchPin)){
            My.GoOnStep(1, true);
        }
        Serial.println("Приехал к концевику");
        delay(1000);
        My.GoOnStep(MMtoStepsY(25),false);
        Serial.println("Ищю линию X");        
        delay(1000);
        while(digitalRead(LineSensorPin)==0){      
            Mx.GoOnStep(1, true);
        }
        Serial.println("Нашёл");
        delay(1000);
        for(int i=0; i<(MMtoStepsX(19.5)-PenOutput); i++){
            Mx.GoOnStep(1, true);
        }
        Xcoord=0;
        Serial.println("Нуль по X, установлен");
        delay(1000);
        for(int i=0; i<MMtoStepsX(15); i++){
            Mx.GoOnStep(1, true);
        }
        int Ylim=MMtoStepsY(160);
        while(digitalRead(LineSensorPin)==0 && Ylim>0){
            My.GoOnStep(1, false,3000);
            Ylim--;
        }
        Serial.println("Линия Y Найдена");
        delay(1000);
        My.GoOnStep(MMtoStepsY(25),false);
        Ycoord=0;
        Serial.println("Нуль по Y установлен, возвращаюсь домой");
        delay(1000);
        for(int i=0; i<MMtoStepsX(10); i++){
            Mx.GoOnStep(1, false);
        }
        Serial.println("Я дома!");
        M0();
        delay(100);
        M1();
    }

    void M1(){
        Pen.write(3);
        Serial.println("Pen up");
    }

    void M0(){
        Pen.write(45);
        Serial.println("Pen down");
    }
    void Down(){
        Mx.GoOnStep(75, false,1000);
    }
    void Up(){
        Mx.GoOnStep(75, true, 1600);
    }

};
