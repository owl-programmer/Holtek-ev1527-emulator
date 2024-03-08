#include "BS86D20A-3.h"
#include "Timer.h"
#include "GPIO.h"

static unsigned int txCode[rxBufferLength];
static unsigned char txCunter;

static void loadRemoteDate(unsigned char remote );


const unsigned long  remoteData[4] =	{	0b001000010001010011100001, //remote data for ch1 ... ch4
											0b001000010001010011100001,
											0b001000010001010011100010,
											0b001000010001010011100010
										};
										

const unsigned int  remoteTiming[][7] ={	
											{6 		//HI part of preamble  sequence 		_|~|_____...
												
											,177 	//LOW part of preamble  sequence    	..-|________|
											
											,6	 	//HI part of 0  sequence				_|~|__..
													
											,17		//Low part of 0  sequence				..-|___|
											
											,17		//HI part of 1  sequence				_|~~~|_..
											
											,6		//Low part of 1  sequence				...~~|_|
											
											,1		//  1 : send wave in form of NOT 		-|_|--------|_|---|___|-|.... 
											
													// 	0 : send Wave Normal 				_|~|________|~|___|~~~|_|....
											},   
											{6	,177	,6	,17	,17	,	6,	0},
											{6	,177	,6	,17	,17	,	6,	1},
											{6	,177	,6	,17	,17	,	6,	0}
										};	



void initTimer(){


	
	_ct0pau	=1; //Counter Pause
	
	_ct0on	=0; //Counter Off
	
	_ct0ck0	=0; // Counter clock source =>  100 => fSUB = 16000000
	_ct0ck1	=0;
	_ct0ck2	=1;
	
	_ct0m0	=0 ; //Select CTM0 Operating Mode = 00: Compare Match Output Mode
	_ct0m1	=0;
	
	_ct0io0	=1; //Select CTP0 output function = 11: Toggle output
	_ct0io1	=1;
	
	_ct0oc	=0; // CTP0 Output control Bit = Initial low;
	
	_ct0pol	=0; //CTP0 Output polarity Control =0: Non-invert
	
	_ct0cclr=1; // Select CTM0 Counter clear condition = 1: CTM0 Comparatror A match
	
	
	_tm0pc1 =1; // select PC7 as CTM0 Output
	
	_ctma0f	=0; //clear CTM0 int. flag
	_ctma0e	=0; //disable CTM0 int.
	_emi	=1; // enable global int. 
	
	_ctm0al		=presendLow; // load dummy value to CTM0;
	_ctm0ah		=presendHI; // load dummy value to CTM0;
	



}

/*
	load remote data to send Buffer
	input: index one of the items on the  remoteData[]; 
*/

static void loadRemoteDate(unsigned char remote ){
	unsigned char	bitCunter;
	unsigned char mask=0;
	
	txCode[0]=remoteTiming[remote][PreambleHi];
	txCode[1]=remoteTiming[remote][PreambleLow];
	
	for (bitCunter=2 ; bitCunter<rxBufferLength; bitCunter+=2){
		
		if ((remoteData[remote]&(0x800000 >> mask))){
			txCode[bitCunter]=remoteTiming[remote][Data1Hi];
			txCode[bitCunter+1]=remoteTiming[remote][Data1Low];			
		}
		else{
			txCode[bitCunter]=remoteTiming[remote][Data0Hi];
			txCode[bitCunter+1]=remoteTiming[remote][Data0Low];	
		}
		mask++;
	
	}	
	
}

/*
	send data of SWs
	input: index one of the items on the  remoteData[]; 
*/

void startSendCode( unsigned char remoteCodeIndex ){
	loadRemoteDate(remoteCodeIndex);
	
	if(remoteTiming[remoteCodeIndex][outPutMode]) 
		_ct0oc	=0;
	else
		_ct0oc	=1;	
		
	_ctm0al		=presendLow;
	_ctm0ah		=presendHI;
	RFON;
	_ct0pau 	=0;
	_ct0on		=1;	
	txCunter	=0;	
	_ctma0e	=1;
}


//stop send data of SWs

void stopSendCode(){
	RFOFF;
	_ctma0e		=0;
	_ct0on		=0;	
	_ct0pau		=1;
}




//CTM0 overFlow int.
void __attribute((interrupt(0x14))) Timer(void)
{	
	_ct0pau=1; //Counter Pause
	_ctma0f=0; //clear CTM0 int. flag
	
	// If all remote bitcode has been sent, it will start sending again
	if (txCunter>=rxBufferLength)
		txCunter=0;
	// Load the next schedule values into the timer	
	_ctm0al=(unsigned char)(txCode[txCunter]);	
	_ctm0ah=(unsigned char)(txCode[txCunter]>>8);
	
	txCunter++;	//Increment pointer for next load


	_ct0pau=0; //Counter Run
	

	
}

