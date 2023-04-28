/***********************

  ************************/
#include "lmosemmctrl.h"
#include "lmosemtypes.h"

void init_halmm(void)
{
	
}

void print_mmapdsc(mach_t *mchp)
{
    mmapdsc_t* map=mchp->mh_mmapdscadr;
    uint_t mnr=mchp->mh_mmapdscnr;
    for(uint_t i=0;i<mnr;i++)
    {
        printfk("mmapdsc[%x].map_phyadr:%x,map_phyadrend:%x\n\r",i,(uint_t)map[i].map_phyadr,(uint_t)map[i].map_phyadrend);
    }
    return;
}


void init_mmapdsc(mach_t *mahp)
{
	phyadrspce_t*adsp = mahp->mh_spedsc;
	uint_t spnr=mahp->mh_spedscnr;
	uint_t mindx=0;

	for (uint_t i=0; i<spnr; i++)
	{
		if (adsp[i].ap_flgs==ADRSPCE_SDRAM)
		{
			mindx=init_core_mmapdsc(adsp[i].ap_adrstart, adsp[i].ap_adrend,mahp->mh_mmapdscadr, mindx);
		}
	}
	mahp->mh_mmapdscadr=mindx;
	mahp->mh_kerinramend=(adr_t)((uint_t)mahp->mh_mmapdscadr+((sizeof(mmapdsc_t)*mindx)));
	return;
}
uint_t init_core_mmapdsc(adr_t adrs, adr_t adre, mmapdsc_t *gmmp,uint_t curmindx)
{
	uint_t mindx = curmindx;
	adr_t tmpadr=adre;

	for(;adrs<adre; adrs+=MAPONE_SIZE,mindx++)
	{
		if ((adrs+MAPONE_SIZE)<adre)
		{
			tmpadr=(adrs+MAPONE_SIZE)-1;
		}
		else
		{
			tmpadr=adre;
		}
		mmapdsc_t_init(&gmmp[mindx], adrs, tmpadr,0,MAP_FLAGES_VAL(0, MAPF_ACSZ_4MB,MAPF_SZ_4MB));
	}

	return mindx;
}

void mmapdsc_t_init(mmapdsc_t *mmp, adr_t phyadr, adr_t phyadre,u32_t allcount,u32_t flags)
{
	list_init(&mmp->map_list);
	hal_spinlock_init(&mmp->map_lock);
	mmp->map_phyadr = phyadr;
	mmp->map_phyadrend = phyadre;
	mmp->map_allcount = allcount;
	mmp->map_flg = flags;
	return;
}
