#include "BS86D20A-3.h"
#include "SYSTEM.h"

static  void initClock();
static void initWDT();
static void initLVDC();



static void initClock()
{
	_fsyson = 1;
	_hircs1 = 1;
	_hircs0 = 0;
	while (!_hto)
		;
	_hlclk = 1;
}
static void initWDT()
{
	_ws0 = 1;
	_ws1 = 1;
	_ws2 = 1;
}
static void initLVDC()
{
	_lvdc = 0x37;
}
void initSystem()
{
	initClock();
	initWDT();
	initLVDC();
}
