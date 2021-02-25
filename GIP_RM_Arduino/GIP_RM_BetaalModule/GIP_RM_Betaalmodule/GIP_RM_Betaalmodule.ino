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
String AllPlayers[13] = {"Admin" , "Blauw1" , "Blauw2" , "Blauw" , "Grijs" , "Zwart" , "Groen" , "Oranje" , "Rood" , "Geel" , "Paars" , "Wit"};
String UIDtags[13] = {" 23 AA E9 1B" , " 5D 68 BD 02" , " 48 FF BF 02" , " 49 3D F8 62", " 21 05 22 03" , " 80 CB 21 03" , " 49 75 D4 02" , " A0 97 21 03" , " DC E8 1A 03" , " 60 7A D3 3D" , " C3 B4 21 03" , " D7 4B 21 03"};
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


LiquidCrystal_I2C lcd(0x27, 16, 2);
////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
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
