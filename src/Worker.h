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
    int PenOutputX=15;  
    int PenOutputY=25; 

    int FormStartX=0;

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

    int MMtoStepsY(float mm){
        return abs(int(5*mm) - 3.1076);
        //return abs(int(5.04*mm - 6.1916));
    }
    int MMtoStepsX(float mm){
        return int(8.445*mm + 3.3797);
    }

    void G28(){
        M1();
        while(digitalRead(LimitSwitchPin)){
            My.GoOnStep(1, true);
        }
        Serial.println("Приехал к концевику");
        delay(1000);

        My.GoOnStep(MMtoStepsY(25),false);//просто подвинулись
        Mx.GoOnStep(MMtoStepsX(70), true,2000);//уезжаем за линию X
        FormStartX=70;
        Serial.println("Ищю линию X");        
        delay(1000);
        int Xlim=MMtoStepsX(100);
        while(digitalRead(LineSensorPin)==0 and Xlim>0){      
            Mx.GoOnStep(1, false,1600);
            FormStartX--;
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
        Dot();
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
    void BackToStart(){
        Mx.GoOnStep(MMtoStepsX(Xcoord+PenOutputX+20), false, 1300);
    }


    void CalibrationTestY(){
        delay(1000);
        M1();
        delay(300);
        while(digitalRead(LimitSwitchPin)){
            My.GoOnStep(1, true);
        }
        M0();
        Serial.println("Приехал к концевику");
        delay(1000);
        My.GoOnStep(125,false);
        for(int i=880;i>520;i-=60){
            M0();
            delay(300);
            My.GoOnStep(i, false,1200);
            delay(500);
            M1();
            delay(300);
            Mx.GoOnStep(MMtoStepsX(5), true,2000);
            delay(300);
            My.GoOnStep(i, true,1200);
            delay(300);
        }
        for(int i=520;i>120;i-=40){
            M0();
            delay(300);
            My.GoOnStep(i, false,1200);
            delay(500);    
            M1();        
            Mx.GoOnStep(MMtoStepsX(5), true,2000);
            delay(500);
            My.GoOnStep(i, true,1200);
            delay(300);
        }
        for(int i=120;i>=20;i-=20){
            M0();
            delay(300);
            My.GoOnStep(i, false,1200);
            delay(500);
            M1();
            delay(300);
            Mx.GoOnStep(MMtoStepsX(5), true,2000);
            delay(300);
            My.GoOnStep(i, true,1200);
            delay(300);
        }
        //880 820 760 700 640 580 520 480 440 400 360 320 280 240 200 160 120 100 80 60 40 20
    }
    
};
