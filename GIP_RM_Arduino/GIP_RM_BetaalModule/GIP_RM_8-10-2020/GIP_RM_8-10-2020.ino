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
String AllPlayers[5] = {"ADMIN" , "Tag" , "Card" , "tesqy"};
String UIDtags[5] = {"49 3D F8 62" , "08 FD 91 B9", "18 37 DF 59", "FA C0 1D 28"};
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
    lcd.print("   Annuleren?   ");                    // Hierna keer je terug naar KEUZEMENU()
    lcd.setCursor(0, 1);
    lcd.print("     Druk #     ");
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



void INLEZEN(bool Stort)                               // leest het aantal CREDITS in van op de badge, gebruikt door BALANS() en STORT()
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
    Serial.print("taagg");

    String tag = "";
    for (byte n = 0; n < mfrc522.uid.size; n++)
    {
      Serial.println(n);
      tag.concat(String(mfrc522.uid.uidByte[n] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[n], HEX));
          Serial.println(tag);

    }
    tag.toUpperCase();
    Serial.println(tag);

    for (byte i = 0; i < 4; i++)
    {
      Serial.println("Forr Lus");
      if (tag.substring(1) == UIDtags[i])    // Als 1 van de UIDtags gelijk
      {
        CurrentPlayer = AllPlayers[i];       // is aan de ingelezen tag
        CorrectRFID = true;                  // Zet CorrectRFID true
        lcd.clear();                         // En verwelkom de speler.
        lcd.print("Welkom ");
        lcd.print(CurrentPlayer);
        Is_Ingelezen = false;
        delay(500);
        tag.remove(0, 12);
        Serial.println(tag);
        KeuzeMenu = true;                    // Zo geraakt het progamma in
        KEUZEMENU();                         // KEUZEMENU()
      }
    }
    if (CorrectRFID == false) {
      Serial.println(tag);
      Serial.println(F("Deze tag zit niet in de databasis"));
      lcd.clear();
      lcd.print(F("Afgewezen"));
      delay(500);
      return;
    }
  }
}
