#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RF24.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int bright_level;
 
/****************** User Config ***************************/
/*** Set this radio as radio number 0 or 1 ***/
bool radioNumber = 0;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/
byte addresses[][6] = {"1Node","2Node"};

void setup () {
  pinMode ( A0, INPUT);  
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
       
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  uint8_t adr[6] = "1Node";
  radio.openWritingPipe(adr);
}

void loop () {
  Serial.println(F("Now sending"));
  
  bright_level = analogRead(A0); 
  
  if (!radio.write( &bright_level, sizeof(int) )){
    Serial.println(F("failed"));
  }
  
  display.clearDisplay();
    
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(bright_level);
  display.display();
  delay(100);
}
