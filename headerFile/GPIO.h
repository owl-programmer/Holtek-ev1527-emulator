#ifndef _GPIO_H
#define _GPIO_H

#include "BS86D20A-3.h"


#define RFON 	_pd0=0
#define RFOFF 	_pd0=1



#define RFkey 			_pc7
 
#define	remote1Key 		_pc6 
#define remote2Key 		_pc5 
#define remote3Key 		_pc4 
#define remote4Key 		_pc3 

#define	remote1 		0 
#define remote2 		1
#define remote3 		2
#define remote4 		3







void initGPIO(void) ;



#endif