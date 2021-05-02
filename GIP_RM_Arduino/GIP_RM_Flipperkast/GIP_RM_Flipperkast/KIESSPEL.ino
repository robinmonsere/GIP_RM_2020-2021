void KIESSPEL()
{
  c_Millis = 0;         //millis gebruikt bij veranderen
  c_PreviousMillis;     //van de keuzemenuState
  cMillis = 0;          //millis gebruikt bij het blinken
  cPreviousMillis = 0;  //van de "<" en ">"
  OnOff = 0;
  int yValue;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Normaal");
  lcd.setCursor(1, 1);
  lcd.print("Battle Royal");
  lcd.setCursor(1, 2);
  lcd.print("Keer terug");

  while (1) {
    cMillis = millis();
    if (cMillis - cPreviousMillis >= blinkInterval) {
      if (OnOff == 0) {
        lcd.setCursor(0, keuzemenuState);
        lcd.print(">");
        lcd.setCursor(19, keuzemenuState);
        lcd.print("<");
        cPreviousMillis = millis();
      }
      if (OnOff == 1) {
        lcd.setCursor(0, keuzemenuState);
        lcd.print(" ");
        lcd.setCursor(19, keuzemenuState);
        lcd.print(" ");
        cPreviousMillis = millis();
      }
      OnOff = !OnOff;
    }
    c_Millis = millis();
    if (c_Millis - c_PreviousMillis >= scrollInterval)
    {
      int yValue = map(analogRead(joystickY), 0, 1023, 0, 100);
      if (yValue <= 45 and keuzemenuState != 0) {
        lcd.setCursor(0, keuzemenuState);
        lcd.print(" ");
        lcd.setCursor(19, keuzemenuState);
        lcd.print(" ");
        keuzemenuState = keuzemenuState - 1;
        OnOff = 0;
        c_PreviousMillis = millis();
      }
      if (yValue >= 55 and keuzemenuState != 2) {
        lcd.setCursor(0, keuzemenuState);
        lcd.print(" ");
        lcd.setCursor(19, keuzemenuState);
        lcd.print(" ");
        keuzemenuState = keuzemenuState + 1;
        OnOff = 0;
        c_PreviousMillis = millis();
      }
    }
    if (keuzemenuState == 0 and digitalRead(joystickSW) == LOW) {
      while (digitalRead(joystickSW) == LOW) {}
      lcd.clear();
      KIESSPEL();
    }
    if (keuzemenuState == 1 and digitalRead(joystickSW) == LOW) {
      while (digitalRead(joystickSW) == LOW) {}
      lcd.clear();
      BATTLEROYAL();
    }
    if (keuzemenuState == 2 and digitalRead(joystickSW) == LOW) {
      while (digitalRead(joystickSW) == LOW) {}
      lcd.clear();
      keuzemenuState = 0;
      KEUZEMENU();
    }
  }
}

