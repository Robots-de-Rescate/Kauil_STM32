#include "delay.h"

void delaybyms(unsigned int j){
	unsigned int k;
	while(j--)
    for(k=10283;k!=0;k--); //10283 7198
}

void delaybyus(unsigned int j){
	unsigned char k;
	while(j--)
    for(k=8;k!=0;k--); //8
}
	
