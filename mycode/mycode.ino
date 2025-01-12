#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#include "binMonitoring.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

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

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Waiting....");
  delay(2000);
  lcd.clear(); 
}

void loop() {
  unsigned long currentMills = millis();

  if(currentMills - prevMills >= interval) {
   
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      lcd.clear();
      if (command.equals(plasticBottle) || command.equals(waterBottle))
      {
        Serial.println("Bottle detected");
        lcd.clear();
        lcd.print("Bottle detected");
        lcd.setCursor(0, 0);

        servoMain.write(0);
        delay(5000);
        servoDischarge.write(45);
        delay(2000);
        servoMain.write(90);
        servoDischarge.write(90);
        delay(1000);
      } else if (command.equals(metalCan)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Metal detected");
        Serial.println("Can detected");

        servoMain.write(180);
        delay(5000);
        servoDischarge.write(45);
        delay(2000);
        servoMain.write(90);
        servoDischarge.write(90);
        delay(1000);
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Non-metal detected");
        Serial.println("Invalid command: " + command);
        servoMain.write(0);
        delay(5000);
        servoDischarge.write(45);
        delay(2000);
        servoMain.write(90);
        servoDischarge.write(90);
        delay(1000);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Waiting...");
    }

    if(bin.canBinDistance() < 30 && bin.can_IRs() == 0) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Bin metal full");
      lcd.setCursor(0,1);
      lcd.print("Action required!");
      bin.buzzerStart();
    }
 
    if(bin.bottleBinDistance() < 30 && bin.bottle_IRs() == 0) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Bin bottle full");
      lcd.setCursor(0,1);
      lcd.print("Action required!");
      bin.buzzerStart();
    }

    prevMills = currentMills;
  }
}
