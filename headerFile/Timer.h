#ifndef _TIMER_H
#define _TIMER_H


/*This project is designed based on EV1527, which has 24 bits of data to send.
 We need to create 48 timing levels, 24 for the waveform at level 1 and 24 for the waveform at level 0.
 plus 2 levels for preamble. => 24+24+2=50*/


#define rxBufferLength 50  

#define PreambleHi 0
#define PreambleLow 1
#define Data0Hi 2 
#define Data0Low 3
#define Data1Hi 4 
#define Data1Low 5
#define outPutMode 6

//#define flyDelay 2 


#define readyToSend 0
#define preparingToSend 1
#define sent 2

#define presendHI  0x00 
#define presendLow 0x50    






void initTimer(void);


void startSendCode(unsigned char remoteCodeIndex);

void stopSendCode(void);

#endif