#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define joyX A0
#define joyY A1
LiquidCrystal_I2C lcd(0x3F, 16, 2);
int xValue;
int yValue;
byte state = 0;
unsigned long c_Millis = 0;         //millis gebruikt bij veranderen
unsigned long c_PreviousMillis;     //van de keuzemenuState
unsigned long cMillis = 0;          //millis gebruikt bij het blinken
unsigned long cPreviousMillis = 0;  //van de "<" en ">"

int blinkInterval = 600;      //ms tussen de '<, >' blinks
int scrollInterval = 0;
bool OnOff = 0;
byte keuzemenuState = 0;
void setup() {
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {
  lcd.setCursor(1, 1); //HIER KOMT >Betalen< Terug<
  lcd.print("Betalen  Terug");
  while (1) {
    cMillis = millis();
    if (cMillis - cPreviousMillis >= blinkInterval) {
      if (OnOff == 0) {
        if (keuzemenuState == 0) lcd.setCursor(0, 1);
        if (keuzemenuState == 1) lcd.setCursor(9, 1);
        lcd.print(">");
        if (keuzemenuState == 0) lcd.setCursor(8, 1);
        if (keuzemenuState == 1) lcd.setCursor(15, 1);
        lcd.print("<");
        cPreviousMillis = millis();
      }
      if (OnOff == 1) {
        if (keuzemenuState == 0) lcd.setCursor(0, 1);
        if (keuzemenuState == 1) lcd.setCursor(9, 1);
        lcd.print(" ");
        if (keuzemenuState == 0) lcd.setCursor(8, 1);
        if (keuzemenuState == 1) lcd.setCursor(15, 1);
        lcd.print(" ");
        cPreviousMillis = millis();
      }
      OnOff = !OnOff;
    }
    //Serial.print("here");
    int xValue = map(analogRead(joyX), 0, 1023, 0, 100);
    if (xValue <= 45 and keuzemenuState == 0) {
      //Serial.println("hereB");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(8, 1);
      lcd.print(" ");

      keuzemenuState = 1;
      OnOff = 0;

    }
    if (xValue >= 55 and keuzemenuState == 1) {
      lcd.setCursor(9, 1);
      lcd.print(" ");
      lcd.setCursor(15, 1);
      lcd.print(" ");

      keuzemenuState = 0;
      OnOff = 0;

    }
  }
}
