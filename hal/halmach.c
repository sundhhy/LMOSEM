/************************************************************************
  LMOSEM HAL 板级系统初始化文件
  *********************************************************************/
#include "lmosemtypes.h"
#include "lmosemmctrl.h" 


void init_halmach()
{

	mach_t_init(&osmach);
	return;
}


void mach_t_init(mach_t *initp)
{
	hal_spinlock_init(&initp->mh_lock);
	list_init(&initp->mh_list);
	initp->mh_kerinramstart = KRNL_INRAM_START;
	initp->mh_kerinramend = (adr_t)(&__end_kernel);
	initp->mh_mmapdscadr = (mmapdsc_t *)(ALIGN(((uint_t)(&__end_kernel)), 4096));
	initp->mh_mmapdscnr = 0;
	initp->mh_spedscnr = PLFM_ADRSPCE_NR;
	initp->mh_spedsc = machadrspce;

	initp->mh_intfltdsc = machintflt;
	initp->mh_intfltnr = IRQ_MAX;
}
