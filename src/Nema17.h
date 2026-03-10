#include <Arduino.h>

class Nema17{
    protected:
    int stepPin;
    int dirPin;
    int enPin;

    public:
    Nema17(int step_pin=0, int dir_pin=0, int en_pin=0){
        stepPin = step_pin;
        dirPin = dir_pin;
        enPin = en_pin;
        pinMode(stepPin, OUTPUT);
        pinMode(dirPin, OUTPUT);
        pinMode(enPin, OUTPUT);
        digitalWrite(enPin, HIGH);
    }
    void OneStep(bool dir, int pulseInterval=1000){
        digitalWrite(dirPin, dir);
        digitalWrite(stepPin, HIGH);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(pulseInterval);    
    }
    void NemaOn(){
        digitalWrite(enPin, LOW);
    }
    void NemaOff(){
        digitalWrite(enPin, HIGH);
    }    
};
class MotorY : public Nema17{
    public:
    MotorY(int step_pin=0, int dir_pin=0, int en_pin=0) : Nema17(step_pin, dir_pin, en_pin){

    };
    void GoOnStep(int StepCount, bool dir,int pulseInterval=1000){
        NemaOn();
        for(int i=0; i<StepCount; i++){
            OneStep(dir, pulseInterval);
        }
        NemaOff();
    }
};

class MotorX{
    private:
    Nema17 m1;
    Nema17 m2;

    public:
    MotorX(int step_pin1=0, int dir_pin1=0, int en_pin1=0, int step_pin2=0, int dir_pin2=0, int en_pin2=0){
        m1 = Nema17(step_pin1, dir_pin1, en_pin1);
        m2 = Nema17(step_pin2, dir_pin2, en_pin2);
    }

    void GoOnStep(int StepCount, bool dir,int pulseInterval=1000){
        m1.NemaOn();
        m2.NemaOn();
        for(int i=0; i<StepCount; i++){
            m1.OneStep(dir, pulseInterval);
            m2.OneStep(!dir, pulseInterval);
        }
    }
};