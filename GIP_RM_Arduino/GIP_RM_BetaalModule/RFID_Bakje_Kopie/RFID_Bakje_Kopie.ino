#include <SPI.h>
#include <MFRC522.h>
#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//////////////////////////////////////////////////////////////////////
byte Block = 60;                  // 60
byte BufferSize = 18;
byte Buffer[18] = "";
bool CorrectRFID = false;
bool KeuzeMenu = true;
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
  Serial.println("TestLoop");
  if (CorrectRFID == false) RFIDSCAN();
  if (CorrectRFID == true)  KEUZEMENU();

}
void KEUZEMENU()
{
  Serial.println("KEUZEMENU");
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

}
void BALANS()
{
  MFRC522::MIFARE_Key key;                              // Maak de key klaar,
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;   // standaard is dit:
  MFRC522::StatusCode status;                           // FFFFFFFFFFFFh

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {                   // check of block klaar is
    lcd.clear();                                        // om te lezen. Wanneer dit
    lcd.setCursor(5, 0);                                // niet zo is, geef Error
    lcd.print(F("Error 404"));                          // door aan monitor en dan
    Serial.print(F("Authentication failed: "));         // return, komt terug in
    Serial.println(mfrc522.GetStatusCodeName(status));  // void KEUZEMENU()
    delay(500);                                         //
    CorrectRFID = false;                                // Zet CorrectRFID = false
    return;                                             // Om opnieuw in RFIDSCAN()
  }                                                     // te komen.
  status = mfrc522.MIFARE_Read(Block, Buffer, &BufferSize);
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
  int NCredits = 0;
  for (byte i = 0; i < 16; i++)
  {
    if (Buffer[i] != 32)
    {
      Serial.println(Buffer[i]);
      NCredits = NCredits + Buffer[i];
    }
  }
  Serial.println(NCredits);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Credits: ");
  lcd.print(NCredits);
  lcd.setCursor(0, 2);
  lcd.print("#: Keer terug");
  char KeyPressed = ' ';
  KeyPressed = keypad.waitForKey();
  if (KeyPressed = '#') {
    KeuzeMenu = true;
    loop();
  }
}


void RFIDSCAN()
{
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
      if (tag.substring(1) == UIDtags[i])
      {
        CurrentPlayer = AllPlayers[i];
        CorrectRFID = true;
        lcd.clear();
        lcd.print("Welkom ");
        lcd.print(CurrentPlayer);
        KeuzeMenu = true;
        delay(500);
      }
    }
  }
}
