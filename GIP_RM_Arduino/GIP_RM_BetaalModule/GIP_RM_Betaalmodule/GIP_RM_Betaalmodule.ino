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

String tag = "";
String CurrentPlayer = "";
String AllPlayers[4] = {"ADMIN" , "Tag" , "Card"};
String UIDtags[4] = {" 49 3D F8 62" , " 08 FD 91 B9", " 23 AA E9 1B"};
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
