void INLEZEN(bool Stort)                               // leest het aantal CREDITS in van op de badge, gebruikt door SALDO() en STORT()
{
  NCredits = 0;                                        // Reset Credits var
  Serial.println(F("Nu in Void INLEZEN"));
  MFRC522::MIFARE_Key key;                             // Maak de key klaar,
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  // standaard is dit:
  MFRC522::StatusCode status;                          // FFFFFFFFFFFFh

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {                   // check of block klaar is
    lcd.clear();                                        // om te lezen. Wanneer dit
    lcd.setCursor(5, 0);                                // niet zo is, geef Error
    lcd.print(F("Error 404"));                          // door aan monitor en dan
    Serial.print(F("Authentication failed: "));         // return, komt terug in
    Serial.println(mfrc522.GetStatusCodeName(status));  // void RFIDSCAN()
    delay(500);                                         //
    CorrectRFID = false;                                // Zet CorrectRFID = false
    return;                                             // Om opnieuw in RFIDSCAN()
  }                                                     // te komen.
  status = mfrc522.MIFARE_Read(Block, ReadBuffer, &BufferSize);
  if (status != MFRC522::STATUS_OK) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print(F("Error 400"));
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    delay(500);
    CorrectRFID = false;
    return;
  }
  Is_Ingelezen = true;                           // Hierdoor lees je maar 1 keer in per kaart.
  for (byte i = 0; i < 16; i++)
  {
    if (ReadBuffer[i] != 32)
    {
      Serial.println(ReadBuffer[i]);
      NCredits = NCredits + ReadBuffer[i];       //aantal Credits op de kaart.
    }
  }
  memset(ReadBuffer, 0, sizeof(ReadBuffer));     // zet de buffer terug leeg voor volgende keer.
  Serial.print("Bool Stort = ");
  Serial.println(Stort);
  if (Stort == false) SALDO();
  

}
