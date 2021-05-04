#include <SD.h>               // need to include the SD library
#define SD_ChipSelectPin 10   //pin10 for Arduino Pro Mini
#include <TMRpcm.h>           //  Lib to play wav file
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

unsigned long time = 0;
int waveform = 0;
int qtyLED = 0;
int timeLED = 25;

void setup(){
  tmrpcm.speakerPin = 9; //pin 9 for Aduino Pro Mini , any pin with PWM fonction will work too
  Serial.begin(9600);

  if (!SD.begin(2)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  else{   
    Serial.println("SD ok");   
  }
  tmrpcm.play("TestSound2.wav"); //the sound file "music" will play each time the arduino powers up, or is reset
  tmrpcm.volume(6); //default volume to 2 
}

void loop(){
 

  if(Serial.available()){    
    switch(Serial.read()){
    case 'd': tmrpcm.play("music.wav"); break; // change music to your file name , for exmple if you file name is susbcribe , the change music.wav to susbcribe.wav , ofcource dont forget to convert your file to vave
                                                 // using online converter
   // you can add  buttons and use if statment to triger some event instead of using the serial commande 
   // for expmle if (pin13 == HIGHT){
   //tmrpcm.play("music.wav");} to play the music when you click on pin 13 , or you can make a counter so you can make a real mp3 player with just 1 button to cycle through  all   songs.
   
    case 'p': tmrpcm.pause(); break;
    case '?': if(tmrpcm.isPlaying()){ Serial.println("A wav file is being played");} break;
    case 'S': tmrpcm.stopPlayback(); break;
    case '=': tmrpcm.volume(1); break;
    case '-': tmrpcm.volume(0); break;
    case '0': tmrpcm.quality(0); break;
    case '1': tmrpcm.quality(1); break;
    default: break;
    }
  }
}
