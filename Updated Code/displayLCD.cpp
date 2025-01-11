#include "displayLCD.h"
#include <Arduino.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void initLCD(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Waiting...");
}

void displayWaste(const char * message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Waste Detected");
  lcd.setCursor(0, 1);
  lcd.println(message)
}

void alarmDisplay(const char * message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message)
  lcd.println(" bin are full");
  lcd.setCursor(0, 1);
  lcd.println("Action Required!")
}

void watingDisplay() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Waiting....");
}

