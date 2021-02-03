#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd(0x27, 20,4);

void setup() {
  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,1);
 }
 
void loop()
{
  delay(1000);
  
  lcd.print("Hello World!"); // Print the string "Hello World!"
 
}
