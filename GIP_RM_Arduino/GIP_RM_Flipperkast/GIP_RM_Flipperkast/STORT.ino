void STORT(int cost)
{
  NCredits = NCredits - cost;
  memset(WriteBuffer, 0, sizeof(WriteBuffer));         // zet de buffer leeg
  for (byte i; i < 16; i++)                            // Zet NCredits om naar de 
  {                                                    // WriteBuffer (array)
    if (NCredits > 255) {
      WriteBuffer[i] = 255;
      NCredits = NCredits - 255;
    }
    else if (NCredits < 255)
    {
      WriteBuffer[i] = NCredits;
      NCredits = 0;
    }
    else if (NCredits = 0)
    {
      WriteBuffer[i] = 0;
    }
  }
  
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
    Is_ingelezen = false;
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
    Is_ingelezen = false;
    return;
  }
  Is_ingelezen = false;
}