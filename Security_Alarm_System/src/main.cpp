#include <Arduino.h>
#include <Keypad.h>

// -------------------------------- Pin definitions/Configs --------------------------------

// sensors
const int SOUND_PIN = 10;

// LED
const int GREEN_LED = 11;
const int ORANGE_LED = 12;
const int WHITE_LED = 13;

// KEYPAD configuration
const byte ROWS = 4; 
const byte COLS = 4;

char keys[ROWS][COLS]={
  {'1', '2', '3', 'A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#', 'D'}
};

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// -------------------------------- Global Variables and States --------------------------------

// Passcode Variables
const String passcode = "3141592";
String enteredCode = "";
bool enteringPasscode = false;

// LED State
bool ledState = false;

// Security System States

enum SecState{ARMED, DISARMED, ALARM, ARMING };

SecState currentState = DISARMED;

// Time Variables
unsigned long startTime = 0;
unsigned long prevBlinkTime = 0;

const unsigned long SETUP_TIME = 5000; 
const unsigned long ALARM_DURATION = 3000; 
const unsigned long BLINK_DURATION = 500;


// Sensor Variables
bool motion = false;
bool sound = false;


// -------------------------------- Function Prototypes --------------------------------
void keypadListener();
void checkPasscode();
void readSound();
void disarmed();
void startArming();
void handleArming();
void handleArmed();
void triggerAlarm();
void handleAlarm();
void disarmSystem();
void disarmedLED();
void armedLED();
void armingLED();
void alarmLED();


// -------------------------------- setup() --------------------------------
void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);

  // Initialize sound sensor pin
  pinMode(SOUND_PIN, INPUT);

  // Initialize LED pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);

  // Initial State 
  currentState = DISARMED;
  disarmedLED();

  // Initial State Output 
  Serial.println("=====================================================");
  Serial.println("SECURITY ALARM SYSTEM INITIALIZED");
  Serial.println("=====================================================");
  Serial.println("System is currently DISARMED.");
  Serial.println("Enter passcode to ARM the system.");  
  
}

// -------------------------------- loop() --------------------------------
void loop() {
  // BE AWARE OF KEYPAD INPUT
  keypadListener();

  // Read sound sensor
  readSound();

  // State processing
  switch (currentState) {
    case DISARMED:
      disarmed();
      break;
    
    case ARMING:
      handleArming();
      break;
    
    case ARMED:
      handleArmed();
      break;

    case ALARM:
      handleAlarm();
      break;
  }

}

void keypadListener(){

  char key = keypad.getKey();
  
  if (key == NO_KEY){
    return;
  }

  // enter passcode mode
  if (key == 'D'){
    enteringPasscode = true;
    Serial.println("====================================");
    Serial.println("Enter passcode:");
    Serial.println("====================================");

    return;
  }

  // Clear passcode
  if (key == '*'){
    enteredCode = "";
    Serial.println("Entered passcode deleted.");
    return;
  }

  // Submit passcode
  if (key == '#'){
    checkPasscode();
    return;
  }

  // Add character to passcode
  enteredCode += key;


  Serial.print("Passcode: ");
  for ( unsigned int i = 0; i < enteredCode.length(); i ++){
    Serial.print("^");
  }
  Serial.println();
}

void checkPasscode(){

  Serial.print("Checking passcode...");

  // Correct passcode 
  if (enteredCode == passcode){
    Serial.println("Correct passcode. System disarmed.");
    enteredCode = "";

    // if system is disarmed, start arming
    if (currentState == DISARMED){
      startArming();
    }
    // if system is armed, disarm it
    else if (currentState == ALARM){
      disarmSystem();
    }
    return;
  }

  // Wrong passcode
  else{
    Serial.println("Incorrect passcode. Intruder Detected.");
    enteredCode = "";
    Serial.println("Enter D to retry.");
  }
  

}

void readSound(){
  sound = digitalRead(SOUND_PIN);

  if (sound){
    Serial.println("Sound detected!");
  }

}

void disarmed(){
  disarmedLED();
}

void startArming(){
  Serial.println("====================================");
  Serial.println("System is now ARMING.");
  Serial.println("====================================");
  Serial.println("You have 5 seconds to leave.");

  currentState = ARMING;
  startTime = millis();
  armingLED();

  prevBlinkTime = millis();
  ledState = false;
}

void handleArming(){
  armingLED();

  unsigned long elapsedTime = millis() - startTime;
  if (elapsedTime >= SETUP_TIME){
    currentState = ARMED;
    Serial.println("====================================");
    Serial.println("System is now ARMED.");
    Serial.println("====================================");

    ledState = false;
    prevBlinkTime = millis();
  }
}

void handleArmed(){
  armedLED();

  if (sound == HIGH){
    Serial.println("====================================");
    Serial.println("INTRUDER DETECTED BY SOUND SENSOR!");
    Serial.println("====================================");

    triggerAlarm();
    return;
  }
}

void triggerAlarm(){
  Serial.println("====================================");
  Serial.println("ALARM TRIGGERED!");
  Serial.println("====================================");

  Serial.println("Enter passcode to disarm the system.");

  currentState = ALARM;
  prevBlinkTime = millis();
  ledState = false;
}

void handleAlarm(){
  alarmLED();
}

void disarmSystem(){
  currentState = DISARMED;

  Serial.println("====================================");
  Serial.println("System is now DISARMED.");
  Serial.println("====================================");

  disarmedLED();
}

void disarmedLED(){
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(ORANGE_LED, LOW);
  digitalWrite(WHITE_LED, LOW);
}

void armedLED(){
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(ORANGE_LED, HIGH);
  digitalWrite(WHITE_LED, LOW);
}

void armingLED(){
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(ORANGE_LED, LOW);

  if (millis() - prevBlinkTime >= BLINK_DURATION){
    prevBlinkTime = millis();
    ledState = !ledState;
    digitalWrite(GREEN_LED, ledState);
  }
}

void alarmLED(){
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(ORANGE_LED, LOW);

  if (millis() - prevBlinkTime >= BLINK_DURATION){
    prevBlinkTime = millis();
    ledState = !ledState;
    digitalWrite(WHITE_LED, ledState);
  }
}

