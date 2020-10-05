<<<<<<< Updated upstream:RFID_Bakje/RFID_Bakje.ino
#include <SPI.h>
#include <MFRC522.h>
#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//////////////////////////////////////////////////////////////////////
bool CorrectRFID = false;
bool KeuzeMenu = true;
bool Is_Ingelezen = false;
//////////////////////////////////////////////////////////////////////
byte Block = 60;                  // 60
byte BufferSize = 18;
byte ReadBuffer[18] = "";
byte WriteBuffer[16] = "";
int NCredits = 0;                //aantal Credits op de kaart.

String CurrentPlayer = "";
String AllPlayers[4] = {"ADMIN" , "Tag" , "Card"};
String UIDtags[4] = {"49 3D F8 62" , "08 FD 91 B9", "18 37 DF 59"};
MFRC522 mfrc522(10, 9);
///////////////////////////////////////////////////////////////////////
const int ROW_NUM = 4;
const int COLUMN_NUM = 3;
char keys[ROW_NUM][COLUMN_NUM] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pin_rows[ROW_NUM] = {8, 7, 6, 5}; // de rijen van het keyboard
byte pin_column[COLUMN_NUM] = {4, 3, 2}; // de kolommen van het keyboard
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
/////////////////////////////////////////////////////////////////////////////////////


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  SPI.begin();          //SPI bus starten
  mfrc522.PCD_Init();   //MFRC init
  MFRC522::StatusCode status;
}

void loop()
{
  Serial.println(F("Nu in Void Loop"));
  if (CorrectRFID == false) RFIDSCAN();
  if (CorrectRFID == true)  KEUZEMENU();

}
void KEUZEMENU()
{
  Serial.println(F("Nu in Void KEUZEMENU"));
  while (KeuzeMenu == true)
  {
    lcd.clear();
    lcd.print("#: Stort Credits");
    lcd.setCursor(0, 2);
    lcd.print("*: Bekijk balans");
    char keypressed = keypad.waitForKey();
    // Serial.print(keypressed);
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
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Druk op # of *"));
      delay(1000);
    }
  }
}
void STORT()
{
  Serial.println(F("Nu in Void STORT"));

  if (Is_Ingelezen == false) INLEZEN(true);              // Leest eerst het aantal Credits in, Dit kan al van BALANS()
  memset(WriteBuffer, 0, sizeof(WriteBuffer));           // zet de buffer leeg
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Bedrag: "));
  lcd.setCursor(0, 2);
  lcd.print(F("Bevestig met *"));

  char PressedKeys[3] = {""};
  byte n = 0;
  bool AsteriskPressed = false;
  while (  AsteriskPressed == false) {
    char PressedKey = ' ';                             // Ingelezen data van keypad in char, makkelijk voor op LCD te zetten.
    PressedKey = keypad.waitForKey();
    if (PressedKey == '*') {
      AsteriskPressed = true;
    }
    else if (n < 3 && PressedKey != '#')
    {
      PressedKeys[n] = PressedKey;
      Serial.println(PressedKeys[n]);
      lcd.setCursor(8 + n, 0);
      lcd.print(PressedKey);
      n = n + 1;
    }
  }
  if ( n == 0)
  {
    lcd.clear();
    lcd.print("   Annuleren?   ");
    lcd.setCursor(0, 2);
    lcd.print("     Druk #     ");
    char KeyPressed = ' ';
    while (KeyPressed != '#') {
      KeyPressed = keypad.waitForKey();
    }
    KeuzeMenu = true;
    loop();
  }


  byte PressedKeyInByte = PressedKeys[0] - '0';
  Serial.println("done");
  Serial.println(PressedKeyInByte);

  char PressedKey = keypad.waitForKey();


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
    KeyPressed = keypad.waitForKey();
  }
  KeuzeMenu = true;
  loop();
}



void INLEZEN(bool Stort)                                // leest het aantal CREDITS in van op de badge, gebruikt door BALANS() en STORT()
{
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
  if (Stort == false) BALANS();
  if (Stort == true)  STORT();


}

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

    String tag = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    tag.toUpperCase();
    for (byte i = 0; i < 3; i++)
    {
      if (tag.substring(1) == UIDtags[i]) {  // Als 1 van de UIDtags gelijk
        CurrentPlayer = AllPlayers[i];       // is aan de ingelezen tag
        CorrectRFID = true;                  // Zet CorrectRFID true
        lcd.clear();                         // En verwelkom de speler.
        lcd.print("Welkom ");
        lcd.print(CurrentPlayer);
        KeuzeMenu = true;                    // Zo geraakt het progamma in
        delay(500);                          // KEUZEMENU()
      }
    }
    if (CorrectRFID == false) {
      Serial.println(F("Deze tag zit niet in de databasis"));
      lcd.clear();
      lcd.print(F("Afgewezen"));
      delay(500);
      return;
    }
  }
}
=======
#include <SPI.h>
#include <MFRC522.h>
#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//////////////////////////////////////////////////////////////////////
bool CorrectRFID = false;
bool KeuzeMenu = true;
bool Is_Ingelezen = false;
//////////////////////////////////////////////////////////////////////
byte Block = 60;                  // 60
byte BufferSize = 18;
byte ReadBuffer[18] = "";
byte WriteBuffer[16] = "";
int NCredits = 0;                //aantal Credits op de kaart.

String CurrentPlayer = "";
String AllPlayers[4] = {"ADMIN" , "Tag" , "Card"};
String UIDtags[4] = {"49 3D F8 62" , "08 FD 91 B9", "18 37 DF 59"};
MFRC522 mfrc522(10, 9);
///////////////////////////////////////////////////////////////////////
const int ROW_NUM = 4;
const int COLUMN_NUM = 3;
char keys[ROW_NUM][COLUMN_NUM] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pin_rows[ROW_NUM] = {8, 7, 6, 5}; // de rijen van het keyboard
byte pin_column[COLUMN_NUM] = {4, 3, 2}; // de kolommen van het keyboard
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
/////////////////////////////////////////////////////////////////////////////////////


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  SPI.begin();          //SPI bus starten
  mfrc522.PCD_Init();   //MFRC init
  MFRC522::StatusCode status;
}

void loop()
{
  Serial.println(F("Nu in Void Loop"));
  if (CorrectRFID == false) RFIDSCAN();
  if (CorrectRFID == true)  KEUZEMENU();

}
void KEUZEMENU()
{
  Serial.println(F("Nu in Void KEUZEMENU"));
  while (KeuzeMenu == true)
  {
    lcd.clear();
    lcd.print("#: Stort Credits");
    lcd.setCursor(0, 2);
    lcd.print("*: Bekijk balans");
    char keypressed = keypad.waitForKey();
    // Serial.print(keypressed);
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
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Druk op # of *"));
      delay(1000);
    }
  }
}
void STORT()
{
  Serial.println(F("Nu in Void STORT"));

  if (Is_Ingelezen == false) INLEZEN(true);              // Leest eerst het aantal Credits in, Dit kan al van BALANS()
  memset(WriteBuffer, 0, sizeof(WriteBuffer));           // zet de buffer leeg
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Bedrag: "));
  lcd.setCursor(0, 2);
  lcd.print(F("Bevestig met *"));

  char PressedKeys[3] = {""};
  byte PressedKeyInByte[3];
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
      PressedKeyInByte[n] = PressedKeys[n] - '0';
      Serial.print("in byte form: ");
      Serial.println(PressedKeyInByte[n]);
      n = n + 1;                                      // houd bij aantal cijfers ingegeven (max 999 dus 3 cijfers)
    }
  }
  if (n == 0)                                         // Wanneer er niks is ingevoerd
  {
    lcd.clear();                                      // word er gevraagd om te annuleren.
    lcd.print("   Annuleren?   ");                    // Hierna keer je terug naar KEUZEMENU()
    lcd.setCursor(0, 2);
    lcd.print("     Druk #     ");
    char KeyPressed = ' ';
    while (KeyPressed != '#') {
      KeyPressed = keypad.waitForKey();
    }
    KeuzeMenu = true;
    KEUZEMENU();
  }


 

  char PressedKey = keypad.waitForKey();


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
    KeyPressed = keypad.waitForKey();
  }
  KeuzeMenu = true;
  loop();
}



void INLEZEN(bool Stort)                                // leest het aantal CREDITS in van op de badge, gebruikt door BALANS() en STORT()
{
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
  if (Stort == false) BALANS();
  if (Stort == true)  STORT();


}

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

    String tag = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    tag.toUpperCase();
    for (byte i = 0; i < 3; i++)
    {
      if (tag.substring(1) == UIDtags[i]) {  // Als 1 van de UIDtags gelijk
        CurrentPlayer = AllPlayers[i];       // is aan de ingelezen tag
        CorrectRFID = true;                  // Zet CorrectRFID true
        lcd.clear();                         // En verwelkom de speler.
        lcd.print("Welkom ");
        lcd.print(CurrentPlayer);
        KeuzeMenu = true;                    // Zo geraakt het progamma in
        delay(500);                          // KEUZEMENU()
      }
    }
    if (CorrectRFID == false) {
      Serial.println(F("Deze tag zit niet in de databasis"));
      lcd.clear();
      lcd.print(F("Afgewezen"));
      delay(500);
      return;
    }
  }
}
>>>>>>> Stashed changes:GIP_RM_BetaalModule/RFID_Bakje/RFID_Bakje.ino
