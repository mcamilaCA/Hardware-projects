#include <Arduino.h>

int pir_pin = 7;
int led_pin = 13; 


void setup() {
  pinMode(pir_pin, INPUT);
  pinMode(led_pin, OUTPUT);

  digitalWrite(led_pin, LOW);
  Serial.begin(9600);
}

void loop() {
  int pir_state = digitalRead(pir_pin);

  if (pir_state == HIGH){
    digitalWrite(led_pin, HIGH);
    Serial.println("There is motion!");

    delay(1000);
  }
  else{
    digitalWrite(led_pin, LOW);
  }
}
