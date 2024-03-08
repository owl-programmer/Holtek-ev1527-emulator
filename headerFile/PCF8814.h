
#ifndef PCF8814_H

#define PCF8814_H

#include "BS86D20A-3.h"







#define CS_LCD_SET    	_pb1=1
#define SCLK_LCD_SET	_pb3=1
#define SDA_LCD_SET		_pb2=1

#define CS_LCD_RESET	_pb1=0
#define SCLK_LCD_RESET	_pb3=0
#define SDA_LCD_RESET	_pb2=0


#define RESET_LCD_SET 		_pb0=1
#define RESET_LCD_RESET	    _pb0=0

#define SET_LCD_BKL		_pb6=1
#define RESET_LCD_BKL	    _pb6=0

#define STE2007_CMD_ONOFF 0xAE
#define STE2007_MASK_ONOFF 0x01

#define STE2007_CMD_DPYREV 0xA6
#define STE2007_MASK_DPYREV 0x01 

#define STE2007_CMD_DPYALLPTS 0xA4
#define STE2007_MASK_DPYALLPTS 0x01

#define STE2007_CMD_LINE 0xB0
#define STE2007_MASK_LINE 0x0F

#define STE2007_CMD_COLMSB 0x10
#define STE2007_MASK_COLMSB 0x07

#define STE2007_CMD_COLLSB 0x00
#define STE2007_MASK_COLLSB 0x0F

#define STE2007_CMD_DPYSTARTLINE 0x40
#define STE2007_MASK_DPYSTARTLINE 0x3F

#define STE2007_CMD_SEGMENTDIR 0xA0
#define STE2007_MASK_SEGMENTDIR 0x01

#define STE2007_CMD_COMDIR 0xC0
#define STE2007_MASK_COMDIR 0x08

#define STE2007_CMD_PWRCTL 0x28
#define STE2007_MASK_PWRCTL 0x07

#define STE2007_CMD_VORANGE 0x40
#define STE2007_MASK_VORANGE 0x07

#define STE2007_CMD_ELECTVOL 0x80
#define STE2007_MASK_ELECTVOL 0x1F

#define STE2007_CMD_RESET 0xE2
#define STE2007_MASK_RESET 0x00

#define STE2007_CMD_NOP 0xE3
#define STE2007_MASK_NOP 0x00

// VOP is set by submitting CMD_VOP, then the value as the next 9-bit byte.
#define STE2007_CMD_VOP 0xE1
#define STE2007_MASK_VOP 0xFF

// VLCD Slope is similar; submit CMD_VLCDSLOPE, then the value in the next 9-bit byte.
#define STE2007_CMD_VLCDSLOPE 0x38
#define STE2007_MASK_VLCDSLOPE 0x07

// Charge Pump similar.
#define STE2007_CMD_CHARGEPUMP 0x3D
#define STE2007_MASK_CHARGEPUMP 0x03

// Refresh Rate similar.
#define STE2007_CMD_REFRESHRATE 0xEF
#define STE2007_MASK_REFRESHRATE 0x03

// Bias ratio is a normal CMD|DATA operation.
#define STE2007_CMD_SETBIAS 0x30
#define STE2007_MASK_SETBIAS 0x07

// N-Line inversion is a compound command (send CMD_NLINEINV, then value)
#define STE2007_CMD_NLINEINV 0xAD
#define STE2007_MASK_NLINEINV 0x1F

// Number of Lines is a normal CMD|DATA operation.
#define STE2007_CMD_NUMLINES 0xD0
#define STE2007_MASK_NUMLINES 0x07

// Image Location is a compound command (send CMD_IMAGELOC, then value)
#define STE2007_CMD_IMAGELOC 0xAC
#define STE2007_MASK_IMAGELOC 0x07

// Icon Mode is a normal CMD|DATA operation.
#define STE2007_CMD_ICONMODE 0xF8
#define STE2007_MASK_ICONMODE 0x01


/* Library functions */

void ste2007_issuecmd(unsigned char, unsigned char, unsigned char);
void ste2007_issue_compoundcmd(unsigned char, unsigned char, unsigned char);
void ste2007_chipselect(unsigned char);

void ste2007_init();
void ste2007_clear();
void ste2007_setxy(unsigned char, unsigned char);   // X is pixel column from 0 to 95, Y is page address from 0 to 8
void ste2007_write(unsigned char, unsigned int); // Write to current DDRAM cursor
void ste2007_invert(unsigned char);  // Invert display (0 = normal, 1 = invert)
void ste2007_powersave(unsigned char); // Powersave (0 = active mode, 1 = powersave ~1-3uA mode)
void ste2007_contrast(unsigned char); // Electronic Volume setting, 0-31 (16 is default)
void ste2007_refreshrate(unsigned char); // Refresh rate of LCD controller, supported values 65/70/75/80, 65Hz is default.
void ste2007_putchar(int, int, const unsigned char  ); /* Write 5 bytes + a zero byte to the specified text
						       * cursor address X coordinate is 0-15 and is multiplied
						       * by 6 to derive graphical column address.
						       * **Specifying a number < 0 (e.g. -1) for either address
						       * skips the ste2007_setxy() call and writes the
						       * character to the current DDRAM cursor address.
						       */
void ste2007_putText(int , int , const  char* );
#endif