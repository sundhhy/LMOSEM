/******************************************
  全局初始化文件halinit.c
  ****************************************/
#include "lmosemtypes.h"
#include "lmosemmctrl.h"

LKHEAD_T void init_hal()
{
	init_paltform();
	init_haluart();
	init_halmach();
	init_halmm();
	init_halintupt();

	return;

}
