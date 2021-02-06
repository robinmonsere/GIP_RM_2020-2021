void KEUZEMENU()
{
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Keuzemenu");
  lcd.setCursor(1, 1);
  lcd.print("Klassement");
  lcd.setCursor(1, 2);
  lcd.print("Bekijk je credits");
  while (1)
  {

    lcd.setCursor(0, keuzemenuState);
    lcd.print(">");
    lcd.setCursor(19, keuzemenuState);
    lcd.print("<");
    delay(700);
    lcd.setCursor(0, keuzemenuState);
    lcd.print(" ");
    lcd.setCursor(19, keuzemenuState);
    lcd.print(" ");
    delay(700);
    Serial.println(analogRead(joystickY));
    if (analogRead(joystickY) >= 750)
    {
      Serial.println("hererere");
      keuzemenuState = keuzemenuState - 1;
    }
    if (analogRead(joystickY) <= 250)
    {
      Serial.println("her");
      keuzemenuState = keuzemenuState + 1;
    }


  }

}
