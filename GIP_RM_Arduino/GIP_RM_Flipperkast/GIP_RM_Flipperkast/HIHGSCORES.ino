void HIGHSCORES() {
  c_Millis = 0;         //millis gebruikt bij veranderen
  c_PreviousMillis;     //van de keuzemenuState
  cMillis = 0;          //millis gebruikt bij het blinken
  cPreviousMillis = 0;  //van de "<" en ">"
  OnOff = 0;
  int yValue;
  keuzemenuState = 1;
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Klassement");
  lcd.setCursor(1, 1);
  lcd.print("Battle Royal");
  lcd.setCursor(1, 2);
  lcd.print("Normaal");
  lcd.setCursor(1, 3);
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
    if (c_Millis - c_PreviousMillis >= scrollInterval) {
      int yValue = map(analogRead(joystickY), 0, 1023, 0, 100);
      if (yValue <= 45 and keuzemenuState != 1) {
        lcd.setCursor(0, keuzemenuState);
        lcd.print(" ");
        lcd.setCursor(19, keuzemenuState);
        lcd.print(" ");
        keuzemenuState = keuzemenuState - 1;
        OnOff = 0;
        c_PreviousMillis = millis();
      }
      if (yValue >= 55 and keuzemenuState != 3) {
        lcd.setCursor(0, keuzemenuState);
        lcd.print(" ");
        lcd.setCursor(19, keuzemenuState);
        lcd.print(" ");
        keuzemenuState = keuzemenuState + 1;
        OnOff = 0;
        c_PreviousMillis = millis();
      }
    }
    if (keuzemenuState == 1 and digitalRead(joystickSW) == LOW) {
      while (digitalRead(joystickSW) == LOW) {}
      Serial.println("1");
      delay(100);
      Klassement_BR();
    }
    if (keuzemenuState == 2 and digitalRead(joystickSW) == LOW) {
      lcd.clear();
    }
    if (keuzemenuState == 3 and digitalRead(joystickSW) == LOW) {  // Gebruiker drukt op 'Keer terug'
      while (digitalRead(joystickSW) == LOW) {}
      Serial.println("1");
      lcd.clear();
      keuzemenuState = 1;
      
      KEUZEMENU();
    }
  }
}

void Klassement_BR() {
  byte i = 1;
  keuzemenuState = 0;
  bool scroll = true;
  while (1) {
    if (scroll == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(i);
      lcd.print(": ");
      lcd.print(Klassement_Players[keuzemenuState] + " met");
      lcd.setCursor(3, 1);
      lcd.print(Klassement_Punten[keuzemenuState]);
      lcd.print(" punten!");
      if (keuzemenuState != 11) {
        lcd.setCursor(0, 2);
        byte n = i + 1;
        lcd.print(n);
        lcd.print(": ");
        lcd.print(Klassement_Players[keuzemenuState + 1] + " met");
        lcd.setCursor(3, 3);
        lcd.print(Klassement_Punten[keuzemenuState + 1]);
        lcd.print(" punten!");
        scroll = false;
      }
      else {
      lcd.setCursor(0, 2);
        lcd.print("Druk op de joystick ");
        lcd.setCursor(0, 3);
        lcd.print("om terug te keren");
        scroll = false;
      }
    }
    c_Millis = millis();
    if (c_Millis - c_PreviousMillis >= scrollInterval) {
      int yValue = map(analogRead(joystickY), 0, 1023, 0, 100);
      if (yValue <= 45 and keuzemenuState != 0) {
        keuzemenuState = keuzemenuState - 1;
        i--;
        c_PreviousMillis = millis();
        scroll = true;
      }
      if (yValue >= 55 and keuzemenuState != 11) {
        keuzemenuState = keuzemenuState + 1;
        i++;
        c_PreviousMillis = millis();
        scroll = true;
      }
    }
    if (digitalRead(joystickSW) == LOW)
    {
      while (digitalRead(joystickSW) == LOW) {delay(10);}
      lcd.clear();
      Serial.println("2");
      HIGHSCORES();
    }
  }
}
void Klassement_NR() {
}


void Klassement_uitrekenen(int Punten) {
  byte i = 0;
  bool gevonden = false;
  byte Old_pointer;
  byte New_pointer;
  while (gevonden == false)             // zoek naar de plaats waar de speler staat in het
  {                                     // klassement met die nieuwe punten
    if (Punten > Klassement_Punten[i])  // punten is globaal
    {
      gevonden = true;
      New_pointer = i;
    }
    if (gevonden == false) i++;
  }
  gevonden = false;
  i = 0;
  while (gevonden == false)  // zoek naar de plaats (pointer) waar de speler nu staat
  {                          // in het klassement
    if (CurrentPlayer == Klassement_Players[i]) {
      gevonden = true;
      Old_pointer = i;
    }
    if (gevonden == false) i++;
  }
  gevonden = false;
  i = 0;
  while (gevonden == false) {
    Klassement_Punten[Old_pointer] = Klassement_Punten[Old_pointer--];
    Klassement_Players[Old_pointer] = Klassement_Players[Old_pointer--];
    Old_pointer--;
    if (Old_pointer == New_pointer) {
      Klassement_Punten[New_pointer] = Punten;
      Klassement_Players[New_pointer] = CurrentPlayer;
      gevonden = true;
    }
  }
}





















