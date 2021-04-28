void RFIDSCAN()
{
  Serial.println(F("Nu in Void RFIDSCAN"));
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print(F("Leg een badge"));
  lcd.setCursor(3, 2);
  lcd.print(F("op de scanner"));
  bool Card = false;                        // lokale var voor check of er een kaard
  while (Card == false) {                   // aanwezig is, dit houd de code tegen
    if (mfrc522.PICC_IsNewCardPresent()) {  // wanneer er geen badge op de lezer ligt
      if (mfrc522.PICC_ReadCardSerial()) {  // Zo wordt niet heel de RFIDSCAN()
        Card = true;                        // doorlopen. Dit bakje staat dan
      }                                     // ook vaak inactief.
    }                                       //
  }                                         //
  while (CorrectRFID == false)
  {
    tag = "";
    Serial.println(tag);
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    tag.toUpperCase();
    Serial.println(tag);
    for (byte i = 0; i < 12; i++)
    {
      if (tag == UIDtags[i])    // Als 1 van de UIDtags gelijk
      {
        CurrentPlayer = AllPlayers[i];       // is aan de ingelezen tag
        CorrectRFID = true;                  // Zet CorrectRFID true
        lcd.clear();                         // En verwelkom de speler.
        lcd.setCursor(3, 0);
        lcd.print(F("Welkom! "));
        lcd.print(CurrentPlayer);
        Is_Ingelezen = false;
        lcd.setCursor(0, 2);
        lcd.print("Druk op de joystick ");
        lcd.setCursor(4, 3);
        lcd.print("om te spelen");
        while (digitalRead(joystickSW) == HIGH)
        {
        }
        delay(500);
        keuzemenuState = 0;
        KeuzeMenu = true;                    // Zo geraakt het progamma in
        KEUZEMENU();                         // KEUZEMENU()
      }
    }
    if (CorrectRFID == false)
    {
      Serial.println(F("Deze tag zit niet in de databasis"));
      lcd.clear();
      lcd.print(F("Afgewezen"));
      delay(500);
      return;
    }
  }
}
