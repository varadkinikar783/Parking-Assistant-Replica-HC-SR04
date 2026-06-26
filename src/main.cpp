#include <Arduino.h>

const uint8_t echopin = 2;
const uint8_t trigpin = 5;
const uint8_t ledpins[5] = {A0, A1, A2, A3, A4}; //A0 == G1, A1 == G2, A2 == Y, A3 == R1, A4 == R2  
const uint8_t readinterval = 50;

volatile unsigned long echostart;
volatile unsigned long echoend;
volatile bool newdistance = false;

unsigned long lastread;
unsigned long lastblink;

void time(){
  if(digitalRead(echopin) == HIGH){
    echostart = micros();
  }
  else if(digitalRead(echopin) == LOW){
    echoend = micros();
    newdistance = true;
  }
}

void setup() {

Serial.begin(115200);
while(!Serial){
  ;
}

  pinMode(echopin, INPUT);
  pinMode(trigpin, OUTPUT);
  digitalWrite(trigpin, LOW);

  for(int i = 0; i <= 4; i++){
    pinMode(ledpins[i], OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(echopin), time, CHANGE);

}

void loop() {
  unsigned long now = millis();
  if(now - lastread >= readinterval){
    lastread = now;
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin, LOW);
  }

  if(newdistance == true){
   long distance = (echoend - echostart) / 58.2;
    Serial.println(distance);
    
    if(distance >0 && distance <= 400){
    if(distance >= 300 && distance <= 400){
      for(int i = 1; i <= 4; i++){
        digitalWrite(ledpins[i], LOW);
      }
      digitalWrite(ledpins[0], HIGH);
      if(now - lastblink >= 1000){
        digitalWrite(ledpins[0], LOW);
        lastblink = now;
      }
    }
    else if(distance < 300 && distance >=250){
      digitalWrite(ledpins[0], HIGH);
        for(int i = 2; i <= 4; i++){
          digitalWrite(ledpins[i], LOW);
        }
        digitalWrite(ledpins[1], HIGH);
        if(now - lastblink >= 800){
          digitalWrite(ledpins[1], LOW);
          lastblink = now;
        }
      }
      else if(distance < 250 && distance >= 100){
        for(int i = 0; i<=1; i++){
          digitalWrite(ledpins[i], HIGH);
        }
        for(int i = 3; i <= 4; i++){
          digitalWrite(ledpins[i], LOW);
        }
        digitalWrite(ledpins[2], HIGH);
        if(now - lastblink >= 500){
          digitalWrite(ledpins[2], LOW);
          lastblink = now;
      }
      } 
      else if(distance < 100 && distance >= 30){
        for(int i = 0; i <= 2; i++){
          digitalWrite(ledpins[i], HIGH);
        }
        digitalWrite(ledpins[4], LOW);
        digitalWrite(ledpins[3], HIGH);
      if(now - lastblink >= 300){
        digitalWrite(ledpins[3], LOW);
        lastblink = now;
      }
      }
      else if(distance < 30){
        for(int i = 0; i <= 3; i++){
          digitalWrite(ledpins[i], HIGH);
        }
        digitalWrite(ledpins[4], HIGH);
      if(now - lastblink >= 70){
        digitalWrite(ledpins[4], LOW);
        lastblink = now;
      }
      }
      newdistance = false;
  }
}
else{
  newdistance = false;
}
}
