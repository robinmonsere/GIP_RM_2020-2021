void BALANS()
{
  Serial.println(F("Nu in Void BALANS"));

  if (Is_Ingelezen == false) INLEZEN(false);            // Leest eerst het aantal Credits in,
  // Dit kan al van STORT()
  Serial.println(NCredits);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Credits: ");
  lcd.print(NCredits);
  lcd.setCursor(0, 2);
  lcd.print("#: Keer terug");
  char KeyPressed = ' ';
  while (KeyPressed != '#') {
    KeyPressed = keypad.getKey();
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
  }
  KeuzeMenu = true;
  loop();
}
