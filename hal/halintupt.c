#include "lmosemtypes.h"
#include "lmosemmctrl.h" 


void init_halintupt()
{
	init_intfltdsc(&osmach);
	return;
}


void init_intfltdsc(mach_t *machp)
{
	intfltdsc_t		*ifdp = machp->mh_intfltdsc;
	uint_t 			ifdnr = machp->mh_intfltnr;
	if (ifdnr > IRQ_MAX)
	{
		hal_sysdie("init intflt err");
	}

	for (uint_t i=MINTNR_START; i<MINTNR_END; i++)
	{
		intfltdsc_t_init(&ifdp[i], MINT_FLG, 0, i-MINT_OFFSET, i);
	}

	for (uint_t i=SINTNR_START; i<SINTNR_END; i++)
	{
		intfltdsc_t_init(&ifdp[i], SINT_FLG, 0, i-SINT_OFFSET, i);
	}

	for (uint_t i=EINTNR_START; i<EINTNR_END; i++)
	{
		intfltdsc_t_init(&ifdp[i], EINT_FLG, 0, i-EINT_OFFSET, i);
	}
}

void intfltdsc_t_init(intfltdsc_t *intp, u32_t flg, u32_t stus, u32_t pbitnr, uint_t irqnr)
{
	hal_spinlock_init(&intp->i_lock);
	list_init(&intp->i_serlist);

	intp->i_sernr = 0;
	intp->i_flg = flg;
	intp->i_stus = stus;
	intp->i_pndbitnr = pbitnr;
	intp->i_irqnr = irqnr;
	intp->i_deep = 0;
	intp->i_indx = 0;

}
