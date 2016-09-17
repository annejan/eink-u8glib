#include <Arduino.h>
#include <SPI.h>
#include "U8glibSSD1608.h"

U8GLIB_SSD1608_296x128 u8g(10, 9);

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22, "Hello World!");
}

void setup(void) {
  // flip screen, if required
  // u8g.setRot180();

  // set SPI backup if required
  u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  u8g.setColorIndex(1);         // pixel on

  pinMode(8, OUTPUT);
}

void loop(void) {
  // picture loop
  u8g.firstPage();
  do {
    draw();
  } while( u8g.nextPage() );

  // rebuild the picture after some delay
  delay(2000);
}
