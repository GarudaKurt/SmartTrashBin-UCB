#include <Wire.h>
#include <Servo.h>
#include <Bonezegei_LCD1602_I2C.h>

#include "binMonitoring.h"

Bonezegei_LCD1602_I2C lcd(0x27);

BinMonitoring bin;

Servo servoMain;
Servo servoDischarge;

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

  servoMain.attach(pinServo_Main);
  servoMain.write(90); //default position

  servoDischarge.attach(pinServo_Discharge);
  servoDischarge.write(90); //default position

  lcd.begin();
  lcd.print("Loading....");
  lcd.setPosition(0,0);
  delay(2000);
  lcd.clear(); 
}

void loop() {
  unsigned long currentMills = millis();

  if(currentMills - prevMills >= interval) {
   
    if(bin.canBinDistance() < 30 && bin.can_IRs() == 0) {
      lcd.clear();
      lcd.setPosition(0,0);
      lcd.print("Bin metal full");
      lcd.setPosition(0,1);
      lcd.print("Action required!");
      bin.buzzerStart();
    }
 
    if(bin.bottleBinDistance() < 30 && bin.bottle_IRs() == 0) {
      lcd.clear();
      lcd.setPosition(0,0);
      lcd.print("Bin bottle full");
      lcd.setPosition(0,1);
      lcd.print("Action required!");
      bin.buzzerStart();
    }

    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      lcd.clear();
      if (command.equals(plasticBottle) || command.equals(waterBottle))
      {
        Serial.println("Bottle detected");
        lcd.print("Bottle detected");
        lcd.setPosition(0, 0);

        servoMain.write(0);
        delay(5000);
        servoDischarge.write(45);
        delay(2000);
        servoMain.write(90);
        servoDischarge.write(90);
        delay(1000);
      } else if (command.equals(metalCan)) {
        lcd.setPosition(0, 0);
        lcd.print("Can detected");
        Serial.println("Can detected");

        servoMain.write(180);
        delay(5000);
        servoDischarge.write(45);
        delay(2000);
        servoMain.write(90);
        servoDischarge.write(90);
        delay(1000);
      } else {
        lcd.setPosition(0, 0);
        lcd.print("Invalid waste detected");
        Serial.println("Invalid command: " + command);
      }
      lcd.clear();
      lcd.setPosition(0, 0);
      lcd.print("Waiting...");
    }

    prevMills = currentMills;
  }
}
