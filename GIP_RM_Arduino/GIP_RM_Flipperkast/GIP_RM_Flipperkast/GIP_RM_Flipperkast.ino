#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>
#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
//////////////////////////////////////////////////////////////////////
#define RST_PIN         9          // Zie pin layout in GIP_RM_RFID
#define SS_PIN          10          // Zie pin layout in GIP_RM_RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522
String tag = "";
String CurrentPlayer = "";
String AllPlayers[13] = {"Admin" , "Blauw1" , "Blauw2" , "Blauw" , "Grijs" , "Zwart" , "Groen" , "Oranje" , "Rood" , "Geel" , "Paars" , "Wit"};
String UIDtags[13] = {" 23 AA E9 1B" , " 5D 68 BD 02" , " 48 FF BF 02" , " 49 3D F8 62", " 21 05 22 03" , " 80 CB 21 03" , " 49 75 D4 02" , " A0 97 21 03" , " DC E8 1A 03" , " 60 7A D3 3D" , " C3 B4 21 03" , " D7 4B 21 03"};
bool CorrectRFID = false;
bool KeuzeMenu = true;
bool Is_ingelezen = false;
bool Klassement_ingelezen_BR = false;
bool Klassement_ingelezen_NR = false;
String Klassement_Players_BR[13];
String Klassement_P_BR[13];
int Klassement_Punten_NR[13];
int Klassement_Punten_BR[13];
String Klassement_Players_NR[13];
String Klassement_P_NR[13];
File NaamBR;
File NaamNR;
File PuntenBR;
File PuntenNR;
//////////////////////////////////////////////////////////////////////
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int flipper_1 = 14;
const int flipper_2 = 15;
const int drukknop_1 = 40;
const int drukknop_2 = 41;

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
unsigned long c_Millis = 0;         //millis gebruikt bij veranderen
unsigned long c_PreviousMillis;     //van de keuzemenuState
unsigned long cMillis = 0;          //millis gebruikt bij het blinken
unsigned long cPreviousMillis = 0;  //van de "<" en ">"
bool OnOff = 0;

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
int blinkInterval = 600;      //ms tussen de '<, >' blinks
int scrollInterval = 300;
//////////////////////////////////////////////////////////////////////
const int stepsPerRevolution = 2048;
Stepper myStepper = Stepper(stepsPerRevolution, 39, 40, 37, 38);
//////////////////////////////////////////////////////////////////////

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
  lcd.setCursor(1, 1);
  pinMode(joystickSW, INPUT_PULLUP);
  if (!SD.begin(4)) {                       // CS pin van SD kaart module
    lcd.print("initialization failed!");
    while (1);
  }
  myStepper.setSpeed(15);
}

void loop()
{
  if (CorrectRFID == false) RFIDSCAN();
}
