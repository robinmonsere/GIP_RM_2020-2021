#include <SPI.h>
#include <MFRC522.h>
#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//////////////////////////////////////////////////////////////////////
#define RST_PIN         49          // Zie pin layout in GIP_RM_RFID
#define SS_PIN          53          // Zie pin layout in GIP_RM_RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522
String tag = "";
String CurrentPlayer = "";
String AllPlayers[13] = {"Admin" , "Blauw1" , "Blauw2" , "Blauw" , "Grijs" , "Zwart" , "Groen" , "Oranje" , "Rood" , "Geel" , "Paars" , "Wit"};
String UIDtags[13] = {" 23 AA E9 1B" , " 5D 68 BD 02" , " 48 FF BF 02" , " 49 3D F8 62", " 21 05 22 03" , " 80 CB 21 03" , " 49 75 D4 02" , " A0 97 21 03" , " DC E8 1A 03" , " 60 7A D3 3D" , " C3 B4 21 03" , " D7 4B 21 03"};
bool CorrectRFID = false;
bool KeuzeMenu = true;
bool Is_Ingelezen = false;
//////////////////////////////////////////////////////////////////////
LiquidCrystal_I2C lcd(0x27, 20,4);

const int flipper_1 = 9;
const int flipper_2 = 10;
const int drukknop_1 = 44;
const int drukknop_2 = 45;

bool stateF1 = LOW;
bool stateF2 = LOW;
bool isUsableF1 = true;
bool isUsableF2 = true;

//////////////////////////////////////////////////////////////////////
byte Block = 60;                  // 60
byte BufferSize = 18;
byte ReadBuffer[18] = "";
byte WriteBuffer[16] = "";
int NCredits = 0;                //aantal Credits op de kaart.

byte keuzemenuState = 0;

unsigned long millisHTL1 = 0;
unsigned long millisHTL2 = 0;
unsigned long previousMillisF1 = 0;
unsigned long previousMillisF2 = 0;
unsigned long currentMillis = 0;
const int timeBetweenUses = 500; // een halve seconde tussen elke flipperactie
const int timerFlipper = 500;

const byte joystickSW = 2;
const byte joystickX = A0;
const byte joystickY = A1;


void setup()
{
  lcd.init();
  lcd.backlight();
  SPI.begin();
  Serial.begin(9600);
  mfrc522.PCD_Init();
  delay(4);
  pinMode(flipper_1, INPUT);
  pinMode(flipper_2, INPUT);
  lcd.setCursor(1,1);

  pinMode(joystickSW, INPUT_PULLUP);
  
 }

void loop()
{
  if (CorrectRFID == false) RFIDSCAN();
   
}
