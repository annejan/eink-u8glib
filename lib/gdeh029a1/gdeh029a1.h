#ifndef GDE_DRIVER_H
#define GDE_DRIVER_H

#include <Arduino.h>

void writeLUT(bool fast);
void initDisplay(bool fast);
void displayImage(const unsigned char *picture);
void setRamArea(unsigned char Xstart,unsigned char Xend,
						    unsigned char Ystart,unsigned char Ystart1,
								unsigned char Yend,unsigned char Yend1);
void setRamPointer(unsigned char addrX,unsigned char addrY,unsigned char addrY1);
void partialDisplay(unsigned char RAM_XST,unsigned char RAM_XEND,unsigned char RAM_YST,
										unsigned char RAM_YST1,unsigned char RAM_YEND,unsigned char RAM_YEND1);
void updateDisplay(void);
void updateDisplayPartial(void);
void writeRam(void);
void powerOff(void);
void powerOn(void);


#endif
