#ifndef _Delay_H
#define _Delay_H
#ifndef __DISABLE_HGCC_BUILD_IN__
#include "build-in.h"
#endif
#define ms1 992
#define ms10 ms1 * 10
#define ms100 ms10 * 10
#define ms1000 ms100 * 10
void msDelay(unsigned int askedDelay);
#endif