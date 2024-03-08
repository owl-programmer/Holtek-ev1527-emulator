
#include "PCF8814.h"
#include "Delay.h"
#include "PCF8814_font.h"



//This library is used https://github.com/spirilis/msp1202 with some changes


void STE2007_SPI_TRANSFER9(unsigned int x){

	unsigned int i,mk=0x100;
	for( i=0 ;i<9;i++){
	  
	  if (x&mk){
	    SDA_LCD_SET;
	  }
	  else
	  {
	    SDA_LCD_RESET;
	    
	  }	
		SCLK_LCD_RESET;
		SCLK_LCD_SET;
		mk>>=1;
	  }
	
}	
// Chip Select line drive; takes 0 or 1 to set the CS line low or high
void  STE2007_CHIPSELECT(unsigned char  x) {
	
	  if (x){
	    CS_LCD_SET;
	  }
	  else
	  {
	    CS_LCD_RESET;
	    
	  }	


}



/* SPI command I/O functions */
void ste2007_issuecmd(unsigned char cmd, unsigned char arg, unsigned char argmask)
{
	STE2007_CHIPSELECT(0);
	STE2007_SPI_TRANSFER9( (unsigned int) (cmd | (arg & argmask)) );
	STE2007_CHIPSELECT(1);
}

void ste2007_issue_compoundcmd(unsigned char cmd, unsigned char arg, unsigned char argmask)
{
	STE2007_CHIPSELECT(0);
	STE2007_SPI_TRANSFER9( (unsigned int)cmd );
	STE2007_SPI_TRANSFER9( (unsigned int) (arg & argmask) );
	STE2007_CHIPSELECT(1);
}

/* A convenience function so the user can configure the Chip Select I/O in one place and use
 * it portable in other libraries stacked on top of this one.
 */
void ste2007_chipselect(unsigned char onoff)
{
	STE2007_CHIPSELECT(onoff);
}

/* Logical LCD operations */
void ste2007_init()
{
	
	_pbc0=0;
	_pbc1=0;
	_pbc2=0;
	_pbc3=0;
	_pbc6=0;
	
	RESET_LCD_RESET;
	msDelay(100);
	RESET_LCD_SET;
	msDelay(100);
	SET_LCD_BKL;
	ste2007_issuecmd(STE2007_CMD_RESET, 0, STE2007_MASK_RESET);  // Soft RESET
	ste2007_issuecmd(STE2007_CMD_DPYALLPTS, 0, STE2007_MASK_DPYALLPTS); // Powersave ALLPOINTS-ON mode turned OFF
	ste2007_issuecmd(STE2007_CMD_PWRCTL, 7, STE2007_MASK_PWRCTL); // Power control set to max
	ste2007_issuecmd(STE2007_CMD_ONOFF, 1, STE2007_MASK_ONOFF); // Display ON
	ste2007_issuecmd(STE2007_CMD_COMDIR, 0, STE2007_MASK_COMDIR); // Display common driver = NORMAL
	ste2007_issuecmd(STE2007_CMD_SEGMENTDIR, 0, STE2007_MASK_SEGMENTDIR); // Lines start at the left
	ste2007_issuecmd(STE2007_CMD_ELECTVOL, 16, STE2007_MASK_ELECTVOL); // Electronic volume set to 16

	ste2007_clear();

	ste2007_issue_compoundcmd(STE2007_CMD_REFRESHRATE, 3, STE2007_MASK_REFRESHRATE); // Refresh rate = 65Hz
	ste2007_issue_compoundcmd(STE2007_CMD_CHARGEPUMP, 0, STE2007_MASK_CHARGEPUMP); // Charge Pump multiply factor = 5x
	ste2007_issuecmd(STE2007_CMD_SETBIAS, 6, STE2007_MASK_SETBIAS); // Bias ratio = 1/4
	ste2007_issue_compoundcmd(STE2007_CMD_VOP, 0, STE2007_MASK_VOP);
	ste2007_issuecmd(STE2007_CMD_DPYREV, 0, STE2007_MASK_DPYREV); // Display normal (not inverted)
}

/* Fully erase DDRAM */
void ste2007_clear()
{
	int i;

	ste2007_setxy(0, 0);
	STE2007_CHIPSELECT(0);
	for (i=0; i < 16*6*9; i++) {
		STE2007_SPI_TRANSFER9( 0x100 );  // Write 0
	}
	STE2007_CHIPSELECT(1);
}

/* Set DDRAM cursor */
void ste2007_setxy(unsigned char x, unsigned char y)
{
	ste2007_issuecmd(STE2007_CMD_LINE, y, STE2007_MASK_LINE);
	ste2007_issuecmd(STE2007_CMD_COLMSB, x >> 4, STE2007_MASK_COLMSB);
	ste2007_issuecmd(STE2007_CMD_COLLSB, x, STE2007_MASK_COLLSB);
}

/* Bulk-write data to DDRAM
 * Note: This function does not drive the Chip Select line but assumes that you will.
 */
void ste2007_write(unsigned char buf, unsigned int len)
{
	unsigned char i , start;
	start= buf-32;

	for (i=0; i < len; i++) {
		STE2007_SPI_TRANSFER9( lcd_Font[start][i] | 0x100 );
	}


}

/* Just for fun... */
void ste2007_invert(unsigned char onoff)
{
	ste2007_issuecmd(STE2007_CMD_DPYREV, onoff, STE2007_MASK_DPYREV);
}

/* STE2007 datasheet lists ONOFF=0, DPYALLPTS=1 as a "Power saver" mode. */
void ste2007_powersave(unsigned char onoff)  // 1 = power-saver mode, 0 = normal mode
{
	ste2007_issuecmd(STE2007_CMD_DPYALLPTS, onoff, STE2007_MASK_DPYALLPTS);
	ste2007_issuecmd(STE2007_CMD_ONOFF, !onoff, STE2007_MASK_ONOFF);
}

/* Set contrast; val is a scale from 0-31 and configures the Electronic Volume setting.
 * 16 is the default.
 */
void ste2007_contrast(unsigned char val)
{
	ste2007_issuecmd(STE2007_CMD_ELECTVOL, val, STE2007_MASK_ELECTVOL);
}

/* Set LCD refresh rate, just for the heck of it.
 * Supported values: 65, 70, 75, 80 (Hz)
 */
void ste2007_refreshrate(unsigned char val)
{
	switch (val) {
		case 80:
			ste2007_issue_compoundcmd(STE2007_CMD_REFRESHRATE, 0, STE2007_MASK_REFRESHRATE);
			break;

		case 75:
			ste2007_issue_compoundcmd(STE2007_CMD_REFRESHRATE, 1, STE2007_MASK_REFRESHRATE);
			break;

		case 70:
			ste2007_issue_compoundcmd(STE2007_CMD_REFRESHRATE, 2, STE2007_MASK_REFRESHRATE);
			break;

		default:
			ste2007_issue_compoundcmd(STE2007_CMD_REFRESHRATE, 3, STE2007_MASK_REFRESHRATE);
	}
}

/* Helper function; write a single 5x8 character at the specified location with the 6th line blank for
 * character spacing.
 * Each byte in *buf is a vertical slice of the row, MSB/LSB orientation depends on SEGMENTDIR parameter.
 *
 * If either x or y are <0, forget setting the cursor and just write it to the current
 *   DDRAM cursor location.
 * x coordinate specified as character position (0-15), not pixel position.  y specified
 *   as character row (page row).
 *
 * This function will drive the Chip Select line since it's meant to be a one-off helper function.
 */
void ste2007_putchar(int x, int y, unsigned char  text)
{
		if (x > -1 && y > -1) {
		ste2007_setxy(x * 6, y);
	}
	
	STE2007_CHIPSELECT(0);
	ste2007_write(text, 6);
	STE2007_CHIPSELECT(1);
}



void ste2007_putText(int x, int y, const char  *text)
{
	unsigned char i;
	if (x > -1 && y > -1) {
		ste2007_setxy(x * 6, y);
	}
	STE2007_CHIPSELECT(0);
	
	for (i=0;text[i];i++){
		ste2007_write(text[i],6);
	}
	STE2007_CHIPSELECT(1);
}