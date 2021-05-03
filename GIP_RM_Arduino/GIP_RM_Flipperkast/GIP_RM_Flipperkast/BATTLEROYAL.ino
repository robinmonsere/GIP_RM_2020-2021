void BATTLEROYAL() {
  Serial.println(F("Nu in Void BATTLEROYAL"));
  c_Millis = 0;         //millis gebruikt bij veranderen
  c_PreviousMillis;     //van de keuzemenuState
  cMillis = 0;          //millis gebruikt bij het blinken
  cPreviousMillis = 0;  //van de "<" en ">"
  OnOff = 0;
  int yValue;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Start het spel");
  lcd.setCursor(1, 1);
  lcd.print("Uitleg");
  lcd.setCursor(1, 2);
  lcd.print("Keer terug");

  /*  
In de volgende While lus kan de gebruiker navigeren op het
scherm met behulp van de joystick
*/
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
    if (keuzemenuState == 0 and digitalRead(joystickSW) == LOW) {  //Gebruiker drukt op
      lcd.setCursor(3, 0);                                         // 'Start het spel'
      lcd.print("Battle Royal");
      lcd.setCursor(1, 1);
      lcd.print("kost: 20 Credits");
      lcd.setCursor(1, 2);
      lcd.print("Saldo:");
      if (Is_ingelezen == false) INLEZEN(true);  // Leest eerst het aantal Credits in
      lcd.setCursor(8, 2);
      lcd.print(NCredits);
      if (NCredits < 20) {                 // als er geen genoeg credits zijn:
        lcd.setCursor(0, 3);               // Saldo ontoereikend! en keert terug
        lcd.print("Saldo ontoereikend!");  // naar keuzemenu met een druk op knop
        while (1)                          // of door badge van scanner te halen
        {
          if (digitalRead(joystickSW) == LOW) KEUZEMENU();
          MFRC522::StatusCode status;                          // Dit stukje is om te checken of de tag
          MFRC522::MIFARE_Key key;                             // nog op de reader ligt, zo niet,
          for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  // dan zal het programma terug in RFIDSCAN() gaan.
          status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Block, &key, &(mfrc522.uid));
          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("Authentication failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            CorrectRFID = false;
            RFIDSCAN();
          }
        }
      } else if (NCredits >= 20) {
        lcd.setCursor(2, 3);               // 
        lcd.print('Betalen  Terug');
        while (1) {
          cMillis = millis();
          if (cMillis - cPreviousMillis >= blinkInterval) {
            if (OnOff == 0) {
              if (keuzemenuState == 0) lcd.setCursor(1, 3);
              if (keuzemenuState == 1) lcd.setCursor(10, 3);
              lcd.print(">");
              if (keuzemenuState == 0) lcd.setCursor(9, 3);
              if (keuzemenuState == 1) lcd.setCursor(16, 3);
              lcd.print("<");
              cPreviousMillis = millis();
            }
            if (OnOff == 1) {
              if (keuzemenuState == 0) lcd.setCursor(1, 3);
              if (keuzemenuState == 1) lcd.setCursor(10, 3);
              lcd.print(" ");
              if (keuzemenuState == 0) lcd.setCursor(9, 3);
              if (keuzemenuState == 1) lcd.setCursor(16, 3);
              lcd.print(" ");
              cPreviousMillis = millis();
            }
            OnOff = !OnOff;
            int xValue = map(analogRead(joystickX), 0, 1023, 0, 100);
            if (xValue <= 45 and keuzemenuState == 0) {
              lcd.setCursor(1, 3);
              lcd.print(" ");
              lcd.setCursor(9, 3);
              lcd.print(" ");
              keuzemenuState = 3;
              OnOff = 0;
            }
            if (xValue >= 55 and keuzemenuState == 1) {
              lcd.setCursor(10, 3);
              lcd.print(" ");
              lcd.setCursor(16, 3);
              lcd.print(" ");
              keuzemenuState = 0;
              OnOff = 0;
            }
            if (keuzemenuState == 0 and digitalRead(joystickSW) == LOW)
            {
              STORT(20);
              // Start het spel
            }
            if (keuzemenuState == 1 and digitalRead(joystickSW) == LOW) KIESSPEL();    // De gebruiker drukt op 'terug'
           
          }
        }
      }
    }
    /*
de gebruiker drukt op 'uitleg' hier kan 
de gebruiker door de uitleg scrollen.
*/
    if (keuzemenuState == 1 and digitalRead(joystickSW) == LOW) {
      bool scrollState = 0;
      do {
        int yValue = map(analogRead(joystickY), 0, 1023, 0, 100);
        if (scrollState == 0) {
          lcd.setCursor(0, 0);
          lcd.print("  In Battle Royal");
          lcd.setCursor(0, 2);
          lcd.print("  moet je proberen");
          lcd.setCursor(0, 3);
          lcd.print("  zo lang mogelijk");
          lcd.setCursor(0, 4);
          lcd.print("  te overleven, je");
          if (yValue >= 55) {
            scrollState == 1;
            lcd.clear();
          }
        }
        if (scrollState == 1) {
          lcd.setCursor(0, 0);
          lcd.print("  moet je proberen");
          lcd.setCursor(0, 2);
          lcd.print("  zo lang mogelijk");
          lcd.setCursor(0, 3);
          lcd.print("  te overleven, je ");
          lcd.setCursor(0, 4);
          lcd.print(" hebt maar 1 leven!");
          if (yValue <= 45) {
            scrollState == 0;
            lcd.clear();
          }
        }
      } while (digitalRead(joystickSW) == HIGH);
    }

    if (keuzemenuState == 2 and digitalRead(joystickSW) == LOW) {  // de gebruiker drukt op 'terug'
      lcd.clear();
      KIESSPEL();
    }
  }
}
