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
    int PenOutputX=18;  
    int PenOutputY=23; 

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
        return abs(int(5.04*mm - 6.1916));
    }
    int MMtoStepsX(float mm){
        return int(8.5028*mm + 3.3797);
    }

    void G28(){
        M1();
        while(digitalRead(LimitSwitchPin)){
            My.GoOnStep(1, true);
        }
        Serial.println("Приехал к концевику");
        delay(1000);

        My.GoOnStep(MMtoStepsY(25),false);//просто подвинулись
        Mx.GoOnStep(MMtoStepsX(70), true,1600);//уезжаем за линию X

        Serial.println("Ищю линию X");        
        delay(1000);
        int Xlim=MMtoStepsX(100);
        while(digitalRead(LineSensorPin)==0 and Xlim>0){      
            Mx.GoOnStep(1, false,1600);
            Xlim--;
        }
        Serial.println("Нашёл");
        Mx.GoOnStep(MMtoStepsX(15), true,1600);//уезжаем за линию, чтобы найти Y
        delay(1000);        
        int Ylim=MMtoStepsY(160);        
        while(digitalRead(LineSensorPin)==0 && Ylim>0){
            My.GoOnStep(1, false,3000);
            Ylim--;
        }
        Serial.println("Линия Y Найдена");
        delay(1000);
        My.GoOnStep(MMtoStepsY(PenOutputY),false);
        Ycoord=0;
        Serial.println("Нуль по Y установлен, возвращаюсь домой");
        delay(1000);
        for(int i=0; i<MMtoStepsX(PenOutputX+15); i++){
            Mx.GoOnStep(1, false,1300);
        }
        Xcoord=0;
        Serial.println("Я дома!");
        M0();
        delay(100);
        M1();
    }

    void M1(){
        Pen.write(5);
        Serial.println("Pen up");
    }
    void M0(){
        Pen.write(45);
        Serial.println("Pen down");
    }
    void Dot(){
        M0();
        M1();
    }
    void Down(){
        Mx.GoOnStep(75, false,1000);
    }
    void Up(){
        Mx.GoOnStep(75, true, 2200);
    }

    void a(){
        // Mx.GoOnStep(MMtoStepsX(30), true,2000);
        // delay(200);
        // M0();
        delay(500);
        // Mx.GoOnStep(MMtoStepsX(32), true,2000);
        // delay(300);
        int b=MMtoStepsY(70);
        My.GoOnStep(b, true,1100);
        delay(1000);
        // Mx.GoOnStep(MMtoStepsX(32), false,1300);
        // delay(400);
        My.GoOnStep(b, false,1100);
        delay(400);
        // M1();
        // delay(300);
        // Mx.GoOnStep(MMtoStepsX(30), false,1300);
        // delay(300);        
        
    }

};
