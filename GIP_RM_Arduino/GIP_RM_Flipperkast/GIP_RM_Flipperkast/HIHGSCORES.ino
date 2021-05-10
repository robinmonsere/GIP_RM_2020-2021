void HIGHSCORES() {
  Serial.println(F("Nu in Void HIGHSCORES"));
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
    MFRC522::StatusCode status;                          // Dit stukje is om te checken of de tag
    MFRC522::MIFARE_Key key;                             // nog op de reader ligt, zoniet,
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  // dan zal het programma terug in RFIDSCAN() gaan.
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      CorrectRFID = false;
      RFIDSCAN();
    }
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
    if (keuzemenuState == 1 and digitalRead(joystickSW) == LOW) {  // Gebruiker drukt op 'Battle Royal'
      while (digitalRead(joystickSW) == LOW) {}
      delay(100);
      Klassement_BR();
    }
    if (keuzemenuState == 2 and digitalRead(joystickSW) == LOW) {  // Gebruiker drukt op 'Normaal'
      while (digitalRead(joystickSW) == LOW) {}                     
      delay(100);
      Klassement_NR();
    }
    if (keuzemenuState == 3 and digitalRead(joystickSW) == LOW) {  // Gebruiker drukt op 'Keer terug'
      while (digitalRead(joystickSW) == LOW) {}
      lcd.clear();
      keuzemenuState = 1;
      KEUZEMENU();
    }
  }
}

void Klassement_BR() {
  Serial.println(F("Nu in Void Klassement_BR"));
  if (Klassement_ingelezen_BR == false) Klassement_SD_R_BR();
  byte i = 1;
  keuzemenuState = 0;
  bool scroll = true;
  while (1) {
    MFRC522::StatusCode status;                          // Dit stukje is om te checken of de tag
    MFRC522::MIFARE_Key key;                             // nog op de reader ligt, zoniet,
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  // dan zal het programma terug in RFIDSCAN() gaan.
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      CorrectRFID = false;
      RFIDSCAN();
    }
    if (scroll == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(i);
      lcd.print(": ");
      lcd.print(Klassement_Players_BR[keuzemenuState] + " met");
      lcd.setCursor(3, 1);
      lcd.print(Klassement_Punten_BR[keuzemenuState]);
      lcd.print(" punten!");
      if (keuzemenuState != 11) {
        lcd.setCursor(0, 2);
        byte n = i + 1;
        lcd.print(n);
        lcd.print(": ");
        lcd.print(Klassement_Players_BR[keuzemenuState + 1] + " met");
        lcd.setCursor(3, 3);
        lcd.print(Klassement_Punten_BR[keuzemenuState + 1]);
        lcd.print(" punten!");
        scroll = false;
      } else {
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
    if (digitalRead(joystickSW) == LOW) {
      while (digitalRead(joystickSW) == LOW) {
        delay(10);
      }
      lcd.clear();
      Serial.println("2");
      HIGHSCORES();
    }
  }
}

void Klassement_NR() {
  Serial.println(F("Nu in Void Klassement_NR"));
  if (Klassement_ingelezen_NR == false) Klassement_SD_R_NR();
  byte i = 1;
  keuzemenuState = 0;
  bool scroll = true;
  while (1) {
    MFRC522::StatusCode status;                          // Dit stukje is om te checken of de tag
    MFRC522::MIFARE_Key key;                             // nog op de reader ligt, zoniet,
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  // dan zal het programma terug in RFIDSCAN() gaan.
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      CorrectRFID = false;
      RFIDSCAN();
    }
    if (scroll == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(i);
      lcd.print(": ");
      lcd.print(Klassement_Players_NR[keuzemenuState] + " met");
      lcd.setCursor(3, 1);
      lcd.print(Klassement_Punten_NR[keuzemenuState]);
      lcd.print(" punten!");
      if (keuzemenuState != 11) {
        lcd.setCursor(0, 2);
        byte n = i + 1;
        lcd.print(n);
        lcd.print(": ");
        lcd.print(Klassement_Players_NR[keuzemenuState + 1] + " met");
        lcd.setCursor(3, 3);
        lcd.print(Klassement_Punten_NR[keuzemenuState + 1]);
        lcd.print(" punten!");
        scroll = false;
      } else {
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
    if (digitalRead(joystickSW) == LOW) {
      while (digitalRead(joystickSW) == LOW) {
        delay(10);
      }
      lcd.clear();
      Serial.println("2");
      HIGHSCORES();
    }
  }
}

void Klassement_uitrekenen(int Punten, bool BR) {
  Serial.println(F("Nu in Void Klassement_uitrekenen"));
  if (BR == true) {
    if (Klassement_ingelezen_BR == false) Klassement_SD_R_BR();
    byte i = 0;
    bool gevonden = false;
    byte Old_pointer;
    byte New_pointer;
    while (gevonden == false)                // zoek naar de plaats waar de speler staat in het
    {                                        // klassement met die nieuwe punten
      if (Punten > Klassement_Punten_BR[i])  // punten is globaal
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
      if (CurrentPlayer == Klassement_Players_BR[i]) {
        gevonden = true;
        Old_pointer = i;
      }
      if (gevonden == false) i++;
    }
    gevonden = false;
    i = 0;
    while (gevonden == false) {
      Klassement_Punten_BR[Old_pointer] = Klassement_Punten_BR[Old_pointer--];
      Klassement_Players_BR[Old_pointer] = Klassement_Players_BR[Old_pointer--];
      Old_pointer--;
      if (Old_pointer == New_pointer) {
        Klassement_Punten_BR[New_pointer] = Punten;
        Klassement_Players_BR[New_pointer] = CurrentPlayer;
        gevonden = true;
      }
    }
  } else if (BR = false) {
    if (Klassement_ingelezen_NR == false) Klassement_SD_R_NR();
    byte i = 0;
    bool gevonden = false;
    byte Old_pointer;
    byte New_pointer;
    while (gevonden == false)                // zoek naar de plaats waar de speler staat in het
    {                                        // klassement met die nieuwe punten
      if (Punten > Klassement_Punten_NR[i])  // punten is globaal
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
      if (CurrentPlayer == Klassement_Players_NR[i]) {
        gevonden = true;
        Old_pointer = i;
      }
      if (gevonden == false) i++;
    }
    gevonden = false;
    i = 0;
    while (gevonden == false) {
      Klassement_Punten_NR[Old_pointer] = Klassement_Punten_NR[Old_pointer--];
      Klassement_Players_NR[Old_pointer] = Klassement_Players_NR[Old_pointer--];
      Old_pointer--;
      if (Old_pointer == New_pointer) {
        Klassement_Punten_NR[New_pointer] = Punten;
        Klassement_Players_NR[New_pointer] = CurrentPlayer;
        gevonden = true;
      }
    }
  }
}

void Klassement_SD_W_BR() {
  Serial.println(F("Nu in Void Klassement_SD_W_BR"));
  // MOET EERST AL INGELEZEN ZIJN
  String StringToWrite;
  for (byte n = 0; n <= 11; n++) {
    StringToWrite += Klassement_Players_BR[n];
    StringToWrite += "+";
  }
  if (SD.exists("NaamBR.txt")) SD.remove("NaamBR.txt");  // De oude file verwijderen
  NaamBR = SD.open("NaamBR.txt", FILE_WRITE);            // Maak nieuwe file aan
  NaamBR.print(StringToWrite);                           // De string met namen wordt
  NaamBR.close();                                        // naar de SD geschreven.

  StringToWrite = "";
  for (byte n = 0; n <= 11; n++) {
    StringToWrite += Klassement_Punten_BR[n];
    StringToWrite += "+";
  }
  Serial.println(StringToWrite);
  if (SD.exists("PuntenBR.txt")) SD.remove("PuntenBR.txt");  // De oude file verwijderen
  PuntenBR = SD.open("PuntenBR.txt", FILE_WRITE);            // Maak nieuwe file aan
  PuntenBR.print(StringToWrite);                             // De string met namen wordt
  PuntenBR.close();                                          // naar de SD geschreven.
}

void Klassement_SD_W_NR() {
  Serial.println(F("Nu in Void Klassement_SD_W_NR"));
  // zelfde als Klassement_SD_W_BR()
}

void Klassement_SD_R_BR() {
  Serial.println(F("Nu in Void Klassement_SD_R_BR"));
  // Hier lees ik de namen in van het klassement BR
  byte i = 0;
  String BUF;  // Lokale buffer om van SD te lezen
  NaamBR = SD.open("NaamBR.txt", FILE_READ);
  if (NaamBR) {
    Serial.println("NaamBR inlezen");
    while (NaamBR.available()) {  // lees tot er niets meer over is
      char ReadBuffer = NaamBR.read();
      if (ReadBuffer == '+') {
        Klassement_Players_BR[i] = BUF;
        Serial.println("-");
        Serial.println(BUF);
        Serial.println(Klassement_Players_BR[i]);
        Serial.println("-");
        BUF = "";
        i++;
      } else {
        BUF += ReadBuffer;
      }
    }
    NaamBR.close();
  } else {  // Als er een error is
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error met SD kaart");
    lcd.setCursor(0, 1);
    lcd.print("550");
    while (1)
      ;
  }
  // Hier lees ik de punten in van het klassement BR
  i = 0;
  BUF = "";
  PuntenBR = SD.open("PuntenBR.txt", FILE_READ);
  if (PuntenBR) {
    Serial.println("PuntenBR inlezen");
    while (PuntenBR.available()) {  // lees tot er niets meer over is
      char ReadBuffer = PuntenBR.read();
      if (ReadBuffer == '+') {
        Klassement_P_BR[i] = BUF;
        BUF = "";
        i++;
      } else {
        BUF += ReadBuffer;
      }
    }
    PuntenBR.close();
    Serial.print("Hier");
    for (byte n = 0; n <= 11; n++) {
      Klassement_Punten_BR[n] = Klassement_P_BR[n].toInt();  // van string naar int
    }
  } else {  // Als er een error is
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error met SD kaart");
    lcd.setCursor(0, 1);
    lcd.print("553");
    while (1)
      ;
  }
  Klassement_ingelezen_BR = true;
}

void Klassement_SD_R_NR() {
  Serial.println(F("Nu in Void Klassement_SD_R_NR"));
  // Hier lees ik de namen in van het klassement NR
  byte i = 0;
  String BUF;  // Lokale buffer om van SD te lezen
  NaamNR = SD.open("NaamNR.txt", FILE_READ);
  if (NaamNR) {
    Serial.println("NaamNR inlezen");
    while (NaamNR.available()) {  // lees tot er niets meer over is
      char ReadBuffer = NaamNR.read();
      if (ReadBuffer == '+') {
        Klassement_Players_NR[i] = BUF;
        BUF = "";
        i++;
      } else {
        BUF += ReadBuffer;
      }
    }
    NaamNR.close();
  } else {  // Als er een error is
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error met SD kaart");
    lcd.setCursor(0, 1);
    lcd.print("551");
    while (1)
      ;
  }
  // Hier lees ik de punten in van het klassement NR
  i = 0;
  BUF = "";
  PuntenNR = SD.open("PuntenNR.txt", FILE_READ);
  if (PuntenNR) {
    Serial.println("PuntenNR inlezen");
    while (PuntenNR.available()) {  // lees tot er niets meer over is
      char ReadBuffer = PuntenNR.read();
      if (ReadBuffer == '+') {
        Klassement_P_NR[i] = BUF;
        Serial.println(BUF);
        Serial.println(Klassement_P_NR[i]);
        BUF = "";
        i++;
      } else {
        BUF += ReadBuffer;
      }
    }
    PuntenNR.close();
    for (byte n = 0; n <= 11; n++) {
      Klassement_Punten_NR[n] = Klassement_P_NR[n].toInt();  // van string naar int
    }
  } else {  // Als er een error is
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error met SD kaart");
    lcd.setCursor(0, 1);
    lcd.print("552");
    while (1)
      ;
  }
  Klassement_ingelezen_NR = true;
}
