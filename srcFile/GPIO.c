#include "GPIO.h"


void	initGPIO(void){
	_acerl	=0x00; // Disable ADC access to port C
	
	_pc7	=1;
	_pc6	=1;
	_pc5	=1;
	_pc4	=1;
	_pc3	=1;
	
	_pcpu7	=1;
	_pcpu6	=1;
	_pcpu5	=1;
	_pcpu4	=1;
	_pcpu3	=1;
	
	_pdc0	=0;
		RFOFF;
	}







	
	
