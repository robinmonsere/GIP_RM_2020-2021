#include <SPI.h>
#include <MFRC522.h>
#include <Key.h>
#include <Wire.h>

MFRC522 mfrc522(10, 9);
byte Block = 60;

void setup()
{
                             
 
  Serial.begin(9600);
  SPI.begin();          //SPI bus starten
  mfrc522.PCD_Init();   //MFRC init
  MFRC522::StatusCode status;
  
 
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent())  return;
  if (!mfrc522.PICC_ReadCardSerial())    return;
   String tag = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

 delay(2000);
 MFRC522::StatusCode status;
 MFRC522::MIFARE_Key key;                             // Maak de key klaar,
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  // standaard is dit:
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    
  }
  Serial.println(status);
  Serial.println(mfrc522.GetStatusCodeName(status));
}
void SCAN()
{
if (!mfrc522.PICC_IsNewCardPresent())  return;
  if (!mfrc522.PICC_ReadCardSerial())    return;
   String tag = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
}
