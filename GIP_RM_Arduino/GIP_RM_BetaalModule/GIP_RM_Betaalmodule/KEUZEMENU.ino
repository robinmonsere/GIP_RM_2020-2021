void KEUZEMENU()
{
  Serial.println(F("Nu in Void KEUZEMENU"));
  lcd.clear();
  lcd.print("#: Stort Credits");
  lcd.setCursor(0, 1);
  lcd.print("*: Bekijk balans");
  while (KeuzeMenu == true)
  {
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
    char keypressed = keypad.getKey();
    if (keypressed == '#')
    {
      KeuzeMenu = false;
      STORT();
    }
    else if (keypressed == '*') // * voor naar balans menu te gaan.
    {
      KeuzeMenu = false;
      BALANS();
    }
    else if (keypressed != NO_KEY)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Druk op # of *"));
      delay(1000);
      lcd.clear();
      lcd.print("#: Stort Credits");
      lcd.setCursor(0, 1);
      lcd.print("*: Bekijk balans");
    }
  }
}
