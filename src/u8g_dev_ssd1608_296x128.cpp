/*

  u8g_dev_ssd1608_296x128.c

*/
#include <u8g.h>
#include <SPI.h>
#include <Arduino.h>

#define WIDTH 296
#define HEIGHT 128
#define PAGE_HEIGHT 8

#define PIN_DATA 8
#define PIN_RESET 12
#define PIN_CS 10


void resetDisplay()
{
  digitalWrite(PIN_RESET, LOW);
  delay(1);
  digitalWrite(PIN_RESET, HIGH);
  delay(1);
}

void writeCommand(unsigned char command)
{
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(command);
  digitalWrite(PIN_CS, HIGH);
}

void writeData(unsigned char data)
{
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
  digitalWrite(PIN_DATA, HIGH);
  SPI.transfer(data);
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_DATA, LOW);
}

void writeCMD_p1(unsigned char command,unsigned char para)
{
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(command);
  digitalWrite(PIN_DATA, HIGH);
  SPI.transfer(para);
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_DATA, LOW);
}

void writeCMD_p2(unsigned char command,unsigned char para1,unsigned char para2)
{
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(command);
  digitalWrite(PIN_DATA, HIGH);
  SPI.transfer(para1);
  SPI.transfer(para2);
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_DATA, LOW);
}

void writeStream(unsigned char *value, unsigned char datalen)
{
  unsigned char i = 0;
	unsigned char *ptemp;

	ptemp = value;
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(*ptemp);
	ptemp++;
  digitalWrite(PIN_DATA, HIGH);
	for(i= 0;i<datalen-1;i++)	// sub the command
	{
		SPI.transfer(*ptemp);
		ptemp++;
	}
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_DATA, LOW);
}

void writeDispRam(unsigned char xSize,unsigned int ySize, const unsigned char *dispdata)
{
  unsigned int i = 0, j = 0, c = 0;
  char data;

	if(xSize%8 != 0)
	{
		xSize = xSize+(8-xSize%8);
	}
	xSize = xSize/8;

  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
	SPI.transfer(0x24);

  digitalWrite(PIN_DATA, HIGH);
	for(i=0;i<ySize;i++)
	{
		for(j=0;j<xSize;j++)
		{
      data = pgm_read_byte(&dispdata[c]);
			SPI.transfer(~data);
      c++;
		}
	}
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_DATA, LOW);
}

void writeDispRamMono(unsigned char xSize,unsigned int ySize, unsigned char dispdata)
{
  unsigned int i = 0,j = 0;
  if(xSize%8 != 0)
  {
  	xSize = xSize+(8-xSize%8);
  }
  xSize = xSize/8;

  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(0x24);

  digitalWrite(PIN_DATA, HIGH);
  for(i=0;i<ySize;i++)
  {
  	for(j=0;j<xSize;j++)
  	{
  	 SPI.transfer(dispdata);
  	}
  }
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_DATA, LOW);
}


const unsigned char LUTDefault_part[30] = {
/*	0x00,0x00,0x00,0x18,
	0x18,0x18,0x18,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x40,0x14,0x34,0x01,
	0x00,0x00,0x00,0x00,
	0x00,0x00,
*/
 0x10 //C221 25C partial update waveform
,0x18
,0x18
,0x08
,0x18
,0x18
,0x08
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00
,0x13
,0x14
,0x44
,0x12
,0x00
,0x00
,0x00
,0x00
,0x00
,0x00

};

const unsigned char LUTDefault_full[30] = {
/*	0x11,0x11,0x10,0x02,	// full
	0x02,0x22,0x22,0x22,
	0x22,0x22,0x51,0x51,
	0x55,0x88,0x08,0x08,
    0x88,0x88,0x00,0x00,
	0x34,0x23,0x12,0x21,
	0x24,0x28,0x22,0x21,
	0xA1,0x01
*/
 0x02 //C221 25C Full update waveform
,0x02
,0x01
,0x11
,0x12
,0x12
,0x22
,0x22
,0x66
,0x69
,0x69
,0x59
,0x58
,0x99
,0x99
,0x88
,0x00
,0x00
,0x00
,0x00
,0xF8
,0xB4
,0x13
,0x51
,0x35
,0x51
,0x51
,0x19
,0x01
,0x00
};


unsigned char GDOControl[]={0x01,0x27,0x01,0x00}; //for 2.9inch
unsigned char softstart[]={0x0c,0xd7,0xd6,0x9d};
unsigned char Rambypass[] = {0x21,0x8f};		// Display update
unsigned char MAsequency[] = {0x22,0xf0};		// clock
unsigned char GDVol[] = {0x03,0x00};	// Gate voltage +15V/-15V
unsigned char SDVol[] = {0x04,0x0a};	// Source voltage +15V/-15V
unsigned char VCOMVol[] = {0x2c,0xa8};	// VCOM 7c
unsigned char BOOSTERFB[] = {0xf0,0x1f};	// Source voltage +15V/-15V
unsigned char DummyLine[] = {0x3a,0x1a};	// 4 dummy line per gate
unsigned char Gatetime[] = {0x3b,0x08};	// 2us per line
unsigned char BorderWavefrom[] = {0x3c,0x33};	// Border
unsigned char RamDataEntryMode[] = {0x11,0x01};	// Ram data entry mode

void writeLUT(bool fast = false)
{
  unsigned char i;
  writeCommand(0x32);//write LUT register
  for (i = 0; i < 30; i++)
  {
    if (fast) {
      writeData(LUTDefault_part[i]);
    } else {
      writeData(LUTDefault_full[i]);
    }
  }
}


void setRamArea(unsigned char Xstart,unsigned char Xend,
						    unsigned char Ystart,unsigned char Ystart1,
                unsigned char Yend,unsigned char Yend1)
{
  unsigned char RamAreaX[3];	// X start and end
	unsigned char RamAreaY[5]; 	// Y start and end
	RamAreaX[0] = 0x44;	// command
	RamAreaX[1] = Xstart;
	RamAreaX[2] = Xend;
	RamAreaY[0] = 0x45;	// command
	RamAreaY[1] = Ystart;
	RamAreaY[2] = Ystart1;
	RamAreaY[3] = Yend;
  RamAreaY[4] = Yend1;
	writeStream(RamAreaX, sizeof(RamAreaX));
	writeStream(RamAreaY, sizeof(RamAreaY));
}

void setRamPointer(unsigned char addrX,unsigned char addrY,unsigned char addrY1)
{
  unsigned char RamPointerX[2];	// default (0,0)
	unsigned char RamPointerY[3];
	RamPointerX[0] = 0x4e;
	RamPointerX[1] = addrX;
	RamPointerY[0] = 0x4f;
	RamPointerY[1] = addrY;
	RamPointerY[2] = addrY1;
	writeStream(RamPointerX, sizeof(RamPointerX));
	writeStream(RamPointerY, sizeof(RamPointerY));
}

void updateDisplay(void)
{
	writeCMD_p1(0x22,0xc7);
	writeCommand(0x20);
	writeCommand(0xff);
}

 void updateDisplayPartial(void)
{
	writeCMD_p1(0x22,0x04);
	//EPD_W21_WriteCMD_p1(0x22,0x08);
	writeCommand(0x20);
	writeCommand(0xff);
}

void powerOn(void)
{
	writeCMD_p1(0x22,0xc0);
	writeCommand(0x20);
//	writeCommand(0xff);
}

void powerOff(void)
{
  writeCMD_p1(0x22,0xc3);
	writeCommand(0x20);
//  writeCommand(0xff);
}

void partialDisplay(unsigned char RAM_XST,unsigned char RAM_XEND,unsigned char RAM_YST,unsigned char RAM_YST1,unsigned char RAM_YEND,unsigned char RAM_YEND1)
 {
   setRamArea(RAM_XST,RAM_XEND,RAM_YST,RAM_YST1,RAM_YEND,RAM_YEND1);  	/*set w h*/
   setRamPointer (RAM_XST,RAM_YST,RAM_YST1);		 /*set orginal*/
 }

// Currently unused functions

void writeRam(void)
{
	writeCommand(0x24);
}

void enableChargepump(void)
{
	writeCMD_p1(0xf0,0x8f);
	writeCMD_p1(0x22,0xc0);
	writeCommand(0x20);
	writeCommand(0xff);
}

void disableChargepump(void)
{
	writeCMD_p1(0x22,0xf0);
	writeCommand(0x20);
	writeCommand(0xff);
}

void initDisplay(bool fast = false)
{
  resetDisplay();
  writeStream(GDOControl, sizeof(GDOControl)); // Pannel configuration, Gate selection
  writeStream(softstart, sizeof(softstart));   // X decrease, Y decrease
  writeStream(VCOMVol, sizeof(VCOMVol));       // VCOM setting
  writeStream(DummyLine, sizeof(DummyLine));   // dummy line per gate
  writeStream(Gatetime, sizeof(Gatetime));     // Gage time setting
  writeStream(RamDataEntryMode, sizeof(RamDataEntryMode));	// X increase, Y decrease
  setRamArea(0x00,0x0f,0x27,0x01,0x00,0x00);	// X-source area,Y-gage area
  setRamPointer(0x00,0xC7,0x00);	// set ram

  writeLUT(fast);
}

void displayImage(const unsigned char *picture)
{
  setRamPointer(0x00,0x27,0x01);	// set ram
	writeDispRam(128, 296, picture);
	updateDisplay();
}

uint8_t u8g_dev_ssd1608_296x128_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
    switch(msg)
    {
        case U8G_DEV_MSG_INIT:
            pinMode(PIN_DATA, OUTPUT);
            initDisplay(true);
            break;

        case U8G_DEV_MSG_STOP:
            break;

        case U8G_DEV_MSG_PAGE_NEXT: {
                u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
                displayImage((const unsigned char *)pb->buf);
                break;
            }

        case U8G_DEV_MSG_SLEEP_ON:
            // u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);
            return 1;

        case U8G_DEV_MSG_SLEEP_OFF:
            // u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);
            return 1;

        // case U8G_DEV_MSG_GET_MODE:
        //     return U8G_MODE_GRAY2BIT;
    }
    // return u8g_dev_pb8h2_base_fn(u8g, dev, msg, arg);
    // return u8g_dev_pb16v2_base_fn(u8g, dev, msg, arg);
    return u8g_dev_pb32h1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_ssd1608_296x128_hw_spi_buf[4736] U8G_NOCOMMON ;
/* frame/page buffer descriptor */
u8g_pb_t u8g_dev_ssd1608_296x128_hw_spi_pb = { {PAGE_HEIGHT, HEIGHT, 0, 0, 0},  WIDTH/2, u8g_dev_ssd1608_296x128_hw_spi_buf};
/* device descriptor */
u8g_dev_t u8g_dev_ssd1608_296x128_hw_spi = { u8g_dev_ssd1608_296x128_fn, &u8g_dev_ssd1608_296x128_hw_spi_pb, u8g_com_null_fn };
