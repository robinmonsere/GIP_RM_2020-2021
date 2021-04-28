void STORT()
{
  
  if (Is_Ingelezen == false) INLEZEN(true);            // Leest eerst het aantal Credits in, Dit kan al van SALDO()
  
  MFRC522::MIFARE_Key key;                             // Maak de key klaar,
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  // standaard is dit:
  MFRC522::StatusCode status;                          // FFFFFFFFFFFFh

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {                   // check of block klaar is
    lcd.clear();                                        // om te schrijven. Wanneer dit
    lcd.setCursor(5, 0);                                // niet zo is, geef Error
    lcd.print(F("Error 500"));                          // door aan monitor en dan
    Serial.print(F("Authentication failed: "));         // return, komt terug in
    Serial.println(mfrc522.GetStatusCodeName(status));  // void RFIDSCAN()
    delay(500);                                         //
    CorrectRFID = false;                                // Zet CorrectRFID = false
    return;                                             // Om opnieuw in RFIDSCAN()
  }                                                     // te komen.
  status = mfrc522.MIFARE_Write(Block, WriteBuffer, 16);
  if (status != MFRC522::STATUS_OK) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print(F("Error 400"));
    Serial.print(F("Writing failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    delay(500);
    CorrectRFID = false;
    return;
  }
}