/**************************************
  CP15 协处理器操作文件
  *********************************/
#include "lmosemtypes.h"
#include "lmosemmctrl.h"


void hal_disable_cache()
{
		__asm__ __volatile__(
			"mcr p15,0,r0,c1,c0,0	\n\t"
			"orr r0,r0,%[tmpdata]	\n\t"
			"mcr p15,0,r0,c1,c0,0	\n\t"
			:
			:[tmpdata] "r" (0x1004)
			:"r0", "cc", "memory"
		);
}
