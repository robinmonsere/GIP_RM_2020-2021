void RFIDSCAN()
{
  Serial.println(F("Nu in Void RFIDSCAN"));
  lcd.clear();
  lcd.setCursor(0, 0);
  //////////"----------------")
  lcd.print(F("Leg je badge"));
  lcd.setCursor(0, 1);
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
    // Serial.println("While Lus");

    tag = "";
        Serial.println(tag);

    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    tag.toUpperCase();
    Serial.println(tag);
    for (byte i = 0; i < 3; i++)
    {
     //  Serial.println(i);
     //  Serial.println(tag.substring(1));
      if (tag == UIDtags[i])    // Als 1 van de UIDtags gelijk
      {  
        CurrentPlayer = AllPlayers[i];       // is aan de ingelezen tag
        CorrectRFID = true;                  // Zet CorrectRFID true
        lcd.clear();                         // En verwelkom de speler.
        lcd.print(F("Welkom "));
        lcd.print(CurrentPlayer);
        Is_Ingelezen = false;
        delay(500);
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
