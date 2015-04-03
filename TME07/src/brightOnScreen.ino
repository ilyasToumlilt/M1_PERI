/**
 * brightOnScreen.ino - Affiche la luminosité captée sur l'ecran.
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas Toumlilt     <toumlilt.ilyas@gmail.com>
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define MAX_BRIGHTNESS 800
#define SCALE_BRIGHT   100

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop()
{
  /* récupération de la valeur de luminosité */
  int brightValue = analogRead(A0) * SCALE_BRIGHT / MAX_BRIGHTNESS;
  
  /* écriture sur l'ecran */
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(brightValue);
  display.display();
  
  delay(100);
}
