// Include required libraries
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
bool CorrectRFID = false;
String CurrentPlayer = "";
MFRC522 mfrc522(10, 9);
String AllPlayers[4] = {"ADMIN" , "Tag" , "Card"};
String UIDtags[4] = {"49 3D F8 62" , "08 FD 91 B9", "18 37 DF 59"};
String tagUID = "49 3D F8 62";


void setup()
{

  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522

  Serial.begin(9600);

}

void loop()
{
  delay(500);
  if (CorrectRFID == false)
  {
    Serial.println("Test");
    RFID();
  }
}

void RFID()
{
  Serial.println("Toon je tag");
  while (CorrectRFID == false)
  {
    Serial.println("While Lus");
    if ( ! mfrc522.PICC_IsNewCardPresent()) {}
    if ( ! mfrc522.PICC_ReadCardSerial()) {}
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
        Serial.println("Acces Granted");
        Serial.println(CurrentPlayer);
      }
    }
  }
}
