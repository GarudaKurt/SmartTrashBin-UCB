#include <Wire.h>
#include <Arduino.h>
#include "displayLCD.h"
#include "binMonitoring.h"
#include "Servos.h"

#define plasticBottle "bottle"
#define waterBottle "waterbottle"
#define metalCan "can"

const long interval = 1000;
unsigned long prevMills = 0;

const int pinServo_Main = 6;
const int pinServo_Discharge = 5;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);

  initLCD();
  initSensors();
  initServo();
}

void loop() {
  unsigned long currentMills = millis();

  if(currentMills - prevMills >= interval) {
   
    if(canBinDistance() < 30 && can_IRs() == 0) {
      alarmDisplay("Non-metal bin");
      buzzerStart();
    } else {
      alarmDisplay("Metal bin full");
      buzzerStop();
    }
 
    if(bottleBinDistance() < 30 && bottle_IRs() == 0) {
      buzzerStart();
    } else {
      buzzerStop();
    }
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      lcd.clear();
      if (command.equals(plasticBottle) || command.equals(waterBottle))
      {
        Serial.println("Bottle detected");
        displayWaste(command.c_str());
        bottleServo();
      } else if (command.equals(metalCan)) {
        displayWaste(command.c_str());
        Serial.println("Can detected");
        canServo();
      } else {
        Serial.println("Invalid command: " + command);
        displayWaste(command.c_str());
        bottleServo();
      }
    }
    waitingDisplay();
    prevMills = currentMills;
  }
}
