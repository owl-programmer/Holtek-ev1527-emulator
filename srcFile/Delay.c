#include "Delay.h"

void msDelay(unsigned int askedDelay)
{

	unsigned int moreDelay;
	for (moreDelay = 0; moreDelay < askedDelay; moreDelay++)
		GCC_DELAY(ms1);
}