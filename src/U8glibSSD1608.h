#include <U8glib.h>

extern u8g_dev_t u8g_dev_ssd1608_296x128_hw_spi;

class U8GLIB_SSD1608_296x128 : public U8GLIB
{
  public:
    U8GLIB_SSD1608_296x128(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_ssd1608_296x128_hw_spi, cs, a0, reset)
      { }
  };
