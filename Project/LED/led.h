#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

void LED_Init(void);

#define LED4_ON  PBout(5) = 0;
#define LED4_OFF PBout(5) = 1;
#define LED2_ON  PEout(5) = 0;
#define LED2_OFF PEout(5) = 1;


#endif
