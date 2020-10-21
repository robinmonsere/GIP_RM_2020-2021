void STORT()
{
  Serial.println(F("Nu in Void STORT"));

  if (Is_Ingelezen == false) INLEZEN(true);              // Leest eerst het aantal Credits in, Dit kan al van BALANS()
  memset(WriteBuffer, 0, sizeof(WriteBuffer));           // zet de buffer leeg
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Bedrag: "));
  lcd.setCursor(0, 1);
  lcd.print(F("Bevestig met *"));

  //  byte PressedKeyInByte[3];
  char PressedKeys[3] = {""};
  byte n = 0;
  bool AsteriskPressed = false;                        // check voor '*', als je hierop duwt
  while ( AsteriskPressed == false) {                  // bevestig je de storting.
    char PressedKey = ' ';                             // Ingelezen data van keypad in char, makkelijk voor op LCD te zetten.
    PressedKey = keypad.waitForKey();
    if (PressedKey == '*') {
      AsteriskPressed = true;
    }
    else if (n < 3 && PressedKey != '#')               // '#' is geen mogelijkheid om door te sturen. (A,B,C,D zijn niet aangesloten);
    {
      PressedKeys[n] = PressedKey;
      Serial.println(PressedKeys[n]);
      lcd.setCursor(8 + n, 0);
      lcd.print(PressedKey);
      //  PressedKeyInByte[n] = PressedKeys[n] - '0';

      n = n + 1;                                      // houd bij aantal cijfers ingegeven (max 999 dus 3 cijfers)
    }
  }
  if (n == 0)                                         // Wanneer er niks is ingevoerd
  {
    lcd.clear();                                      // word er gevraagd om te annuleren.
    lcd.print(F("   Annuleren?   "));                 // Hierna keer je terug naar KEUZEMENU()
    lcd.setCursor(0, 1);
    lcd.print(F("     Druk #     "));
    char KeyPressed = ' ';
    while (KeyPressed != '#') {
      KeyPressed = keypad.waitForKey();
    }
    KeuzeMenu = true;
    KEUZEMENU();
  }

  NCredits = NCredits + atoi(&PressedKeys[0]);        // berekenen van aantal totale credits

  if (NCredits > 999999)                              // als dit groter is dan 999999 geef een error
  {
    lcd.clear();
    lcd.print(F("   Max aantal"));
    lcd.setCursor(0, 1);
    lcd.print(F("Credits: 999999"));
    delay(2000);
    NCredits = 0;                                    // zet Credits terug naar 0
    Is_Ingelezen = false;                            // hierdoor word alles volgende keer ingelezen
    KeuzeMenu = true;                                // terug in KEUZEMENU()
    KEUZEMENU();
  }

  lcd.clear();                                       // Nog éénmaal bevestigen.
  lcd.print(F("Saldo:"));
  lcd.setCursor(7, 0);
  lcd.print(NCredits);
  lcd.setCursor(0, 1);
  lcd.print(F("Bevestig met *"));
  char  KeyPressed = ' ';
  while (KeyPressed != '*') {
    KeyPressed = keypad.waitForKey();
  }

  Is_Ingelezen = false;                             // Na bevesteging zet Is_Ingelezen op false, zodanig word dit opnieuw ingelezen.

  for (byte i; i < 16; i++)
  {
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
    Serial.print(i);
    Serial.print(": ");
    Serial.println(WriteBuffer[i]);
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
  if (status == MFRC522::STATUS_OK) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print(F("Storting"));
    lcd.setCursor(4, 1);
    lcd.print(F("geslaagd"));
    delay(2000);
    KeuzeMenu = true;                                   // breng het programma terug naar
    KEUZEMENU();                                        // KEUZEMENU()
  }
}
