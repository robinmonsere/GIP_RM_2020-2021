
#include <SPI.h>
#include <SD.h>

File myFile;

byte  i = 0;
String TEST[13];
String BUF;
String Klassement_Players[13] = {"Admin" , "Blauw1" , "Blauw2" , "Blauw" , "Grijs" , "Zwart" , "Groen" , "Oranje" , "Rood" , "Geel" , "Paars" , "Wit"};

void setup() {
  Serial.begin(9600);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }

  if (SD.exists("TEST.txt"))
  {
    SD.remove("TEST.txt");
  }
  myFile = SD.open("TEST.txt", FILE_WRITE);
  String StringToWrite;
  for (byte n = 0; n <= 11; n++)
  {
    StringToWrite += Klassement_Players[n];
    StringToWrite += "+";
    Serial.println(StringToWrite);
  }
  myFile.print(StringToWrite);
  myFile.close();

  myFile = SD.open("TEST.txt", FILE_READ);

  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      char ReadBuffer = myFile.read();
      //Serial.println(ReadBuffer);
      if (ReadBuffer == '+')
      {
        TEST[i] = BUF;
        Serial.println("-");
        Serial.println(BUF);
        Serial.println(TEST[i]);
        Serial.println("-");
        BUF = "";
        i++;
        //Serial.println(i);
      }
      else
      {
        BUF += ReadBuffer;
        Serial.println(BUF);
      }
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  Serial.println(i);
  Serial.println("VANAF HIER");

  for (int n = 0; n <= 10; n++) {
    //Serial.print(n);
    Serial.println(TEST[n]);
    delay(100);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
