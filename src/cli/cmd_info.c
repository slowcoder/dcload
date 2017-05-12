#include <stdint.h>

#include "log.h"

extern volatile uint32_t uNumIRQs;

int clicmd_INFO(int argc,char **argv) {
	uint16_t *pFS;
	uint32_t *pUID;

    LOG("Num IRQs: %lu",uNumIRQs);

#if 1 // F10x addresses
	pFS = (uint16_t*)0x1FFFF7E0;
	LOG("Flash size: %uKB",pFS[0]);

	pUID = (uint32_t*)0x1FFFF7E8;
	LOG("Device UID: %08lx%08lx%08lx",pUID[2],pUID[1],pUID[0]);
#endif
	return 0;
}
