#include "BS86D20A-3.h"
#include "SYSTEM.h"
#include "GPIO.h"
#include "Timer.h"
#include "Delay.h"
#include "PCF8814.h"

void main()
{
	static unsigned char sendingStatus=0;
	 		unsigned char inuseRemote;
	initSystem();

	initGPIO();
	ste2007_init();
	initTimer();
	
	ste2007_putText(0,0,"|--------------|");
	ste2007_putText(0,1,"|Remote Emulatr|"); // The "Emulator" is correct, but it does not fit on the screen ;-)
	ste2007_putText(0,2,"|______________|");
	ste2007_putText(0,4,"RF Power:OFF");
	ste2007_putText(0,5,"Key Pressed:*");	
	while (1)
	{

		//If the RF key is pressed, it checks the status of the remote key
		if (!RFkey){
			if (sendingStatus!=sent){
				sendingStatus=preparingToSend;
				msDelay(500);
				inuseRemote=0xAA;
				if(!remote1Key)
					inuseRemote=remote1;
				else if (!remote2Key)
					inuseRemote=remote2;
				else if (!remote3Key)
					inuseRemote=remote3;
				else if (!remote4Key)
					inuseRemote=remote4;			
				
				ste2007_putText(0,4,"RF Power:ON ");
					
				if (inuseRemote!=0xAA){
					startSendCode(inuseRemote);
					sendingStatus=0x01	;
					sendingStatus=sent;
					ste2007_putText(0,5,"Key Pressed:");
					ste2007_putchar(12,5,inuseRemote+49);
				}	
			}	
		}
		else{
			if (sendingStatus == sent){
				stopSendCode();
				sendingStatus=0;
				msDelay(500);
				sendingStatus=readyToSend;
				ste2007_putText(0,4,"RF Power:OFF");
				ste2007_putText(0,5,"Key Pressed:*");	
			}
			else if (sendingStatus==preparingToSend ){
				sendingStatus=readyToSend;
				ste2007_putText(0,4,"RF Power:OFF");
			}
				
		}
		
		GCC_CLRWDT();
	}
}