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
    int PenOutputX=16;  
    int PenOutputY=21; 

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
        delay(1500);
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
        delay(300);
        M1();
        delay(200);
    }
    void Down(){
        Mx.GoOnStep(MMtoStepsX(10), false,1000);
        Xcoord-=10;
    }
    void Up(){
        Mx.GoOnStep(MMtoStepsX(10), true, 2200);
        Xcoord+=10;
    }
    void GoTo(int x, int y){
        float Ydv=abs(y-Ycoord);
        float Xdv=abs(x-Xcoord);
        bool xdir=x>Xcoord;
        bool ydir=y<Ycoord;
        M1();
        delay(300);
        My.GoOnStep(MMtoStepsY(Ydv), ydir,1500);
        Ycoord=y;
        delay(200);
        Mx.GoOnStep(MMtoStepsX(Xdv), xdir,2000);
        Xcoord=x;
        delay(200);
    }
    void G1(int x,int y){
        M0();
        delay(300);
        float Ydv=abs(y-Ycoord);
        float Xdv=abs(x-Xcoord);
        bool xdir=x>Xcoord;
        bool ydir=y<Ycoord;
        if(Ydv!=0 && Xdv!=0){
            int last=0;
            if(MMtoStepsY(Ydv)<MMtoStepsX(Xdv)){
                float tg=float(MMtoStepsY(Ydv))/float(MMtoStepsX(Xdv));
                for(int i=0; i<MMtoStepsX(Xdv); i++){
                    Mx.GoOnStep(1, xdir,2000);
                    if(int(tg*i)==last+1){
                        last=int(tg*i);
                        My.GoOnStep(1, ydir,1700,false);
                    }
                }
            }
            else{
                float tg=float(MMtoStepsX(Xdv))/float(MMtoStepsY(Ydv));
                for(int i=0; i<MMtoStepsY(Ydv); i++){
                    My.GoOnStep(1, ydir,1700,false);
                    if(int(tg*i)==last+1){
                        last=int(tg*i);
                        Mx.GoOnStep(1, xdir,2000);
                    }
                }
            }

            My.NemaOff();
        }
        else{
            if(Ydv==0){
                My.NemaOn();
                Mx.GoOnStep(MMtoStepsX(Xdv), xdir,2000);
                My.NemaOff();
            }
            else{
                My.GoOnStep(MMtoStepsY(Ydv), ydir,1700);
            }
        }
        Xcoord=x;
        Ycoord=y;
        delay(200);
        M1();
    }

    void a(){ 
        M0();
        delay(300); 
        My.GoOnStep(MMtoStepsY(30), false,2000);
        M1();
    }
};
