/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

File myFile;
char FileInfo[109];
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(2)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");


  myFile = SD.open("test.txt");
  if (myFile)
  {
    Serial.println("test.txt:");
    int i = 0;
    while (myFile.available()) {
      //Serial.write(myFile.read());
      FileInfo[i] = myFile.read();
      i++;
      Serial.println(i);
      delay(10);
    }
    // close the file:
    myFile.close();
    delay(100);
    Serial.println("File Closed");
    Serial.println(i);
    Serial.print(FileInfo[18]);
  /*  for (i; i >= 0; i = i - 1)
    {
      Serial.print(i + ":");
      Serial.println(FileInfo[i]);

    }  */
  }

}

void loop() {
  // nothing happens after setup
}
