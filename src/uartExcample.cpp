#include <SoftwareSerial.h>
#include <arduino.h>

SoftwareSerial myDevice(10, 11); //RX,TX

void setup() {
  Serial.begin(9600); 
  myDevice.begin(9600); 
}

void loop() {
  if (myDevice.available()) {
    char c = myDevice.read();
    Serial.print(c);
  }
}