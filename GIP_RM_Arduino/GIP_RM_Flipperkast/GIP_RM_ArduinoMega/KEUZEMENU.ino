void KEUZEMENU()
{
  unsigned long cMillis = 0;
  unsigned long cPreviousMillis = 0;
  const int interval = 600;
  bool OnOff = 0;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Keuzemenu");
  lcd.setCursor(1, 1);
  lcd.print("Klassement");
  lcd.setCursor(1, 2);
  lcd.print("Bekijk je credits");

  while (1)
  {
    cMillis = millis();
    if (cMillis - cPreviousMillis >= interval)
    {
      if (OnOff == 0)
      {
        lcd.setCursor(0, keuzemenuState);
        lcd.print(">");
        lcd.setCursor(19, keuzemenuState);
        lcd.print("<");
        cPreviousMillis = millis();
      }
      if (OnOff == 1)
      {
        lcd.setCursor(0, keuzemenuState);
        lcd.print(" ");
        lcd.setCursor(19, keuzemenuState);
        lcd.print(" ");
        cPreviousMillis = millis();
      }
      OnOff = !OnOff;
    }

    if (analogRead(joystickY) >= 750 and keuzemenuState != 0)
    {
      lcd.setCursor(0, keuzemenuState);
      lcd.print(" ");
      lcd.setCursor(19, keuzemenuState);
      lcd.print(" ");
      keuzemenuState = keuzemenuState - 1;
      OnOff = 0;
    }
    if (analogRead(joystickY) <= 250 and keuzemenuState != 2)
    {
      lcd.setCursor(0, keuzemenuState);
      lcd.print(" ");
      lcd.setCursor(19, keuzemenuState);
      lcd.print(" ");
      keuzemenuState = keuzemenuState + 1;
      OnOff = 0;
    }
    delay(200);
    if (keuzemenuState == 2 and digitalRead(joystickSW) == LOW)
    {
      lcd.clear();
      BALANS();
    }
  }

}
