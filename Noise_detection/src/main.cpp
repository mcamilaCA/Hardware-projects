#include <Arduino.h>

int sensor = 2; // pin in which the sound sensor is
int led = 13; // pin where the led (positive side) is connected
boolean sensorState = false; // state of the led is off by default 


void setup() {
  pinMode(sensor, INPUT);
  pinMode(led, OUTPUT); 
}

void loop() {
  int data = digitalRead(sensor); // getting the state of the sound sensor

  // if noise is detected 
  if (data == 1){
    if(sensorState == true){
      sensorState = LOW; // changing the state of led since there is noise
      digitalWrite(led, LOW); // turning on led light
    }
    else{
      sensorState = true; // if the led is low, turn it on
      digitalWrite(led, HIGH); // turning off led light
    }
  }
}
