#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "log.h"

int clicmd_UNAME(int argc,char **argv) {
	uint32_t v;

//	v = HAL_GetHalVersion();
	v = 0;
#if 1
	LOG("FreeRTOS %s - Built %s %s",
		tskKERNEL_VERSION_NUMBER,
		__DATE__,
		__TIME__);
#else
	LOG("Rev %s - FreeRTOS %s - STM32 HAL v%u.%u.%u - Built %s %s",
		GIT_BASE_REVISION,
		tskKERNEL_VERSION_NUMBER,
		(v >> 24) & 0xFF,
		(v >> 16) & 0xFF,
		(v >>  8) & 0xFF,
		__DATE__,
		__TIME__);
#endif
	return 0;
}
