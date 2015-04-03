#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include "RF24.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int bright_level;
 
/****************** User Config ***************************/
/*** Set this radio as radio number 0 or 1 ***/
bool radioNumber = 1;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

void setup () {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
       
  radio.begin();
  uint8_t adr[6] = "1Node";
  radio.openReadingPipe(1, adr);
    
  radio.startListening();
  Serial.println(F("Initialized"));
}

void loop () {
  if( radio.available()){
    Serial.println(F("in"));
    // Variable for the received timestamp
    radio.read( &bright_level, sizeof(int) ); // Get the payload
    Serial.println(bright_level);
      
    display.clearDisplay();
    
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(bright_level);
    display.display();
    delay(100);
  }  
}
