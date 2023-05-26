/***********************

  ************************/
// 注意：2个头文件包含顺序不能乱
#include "lmosemtypes.h"
#include "lmosemmctrl.h"

void init_halmm(void)
{
	
	if (!cheksum_phyadrspce(&osmach))
	{
		hal_sysdie("cheksum_phyaddrspace failed!");
	}
	init_mmapdsc(&osmach);
	init_phymem();

	onmmapdsc_inkrlram(&osmach, &osphymem);
	print_mmapdsc(&osmach);
}

void print_mmapdsc(mach_t *mchp)
{
    mmapdsc_t* map=mchp->mh_mmapdscadr;
    uint_t mnr=mchp->mh_mmapdscnr;
	printfk(" mnr=%d\n\r", mnr);
    for(uint_t i=0;i<mnr;i++)
    {
        printfk("mmapdsc[%x] map_phyadr:%x - %x \n\r",i,(uint_t)map[i].map_phyadr,(uint_t)map[i].map_phyadrend);
    }
    return;
}


//SDRAM存在 kernei位于SDRAM中则返回TRUE
sint_t cheksum_phyadrspce(mach_t *mahp)
{
	sint_t sdramsts = FALSE, kerinmmsts = FALSE;
	phyadrspce_t *adsp = mahp->mh_spedsc;
	uint_t spnr = mahp->mh_spedscnr;

	if (!adsp ||spnr != PLFM_ADRSPCE_NR)
	{
		printfk("err head\n\r");
		return FALSE;
	}

	//检查最后一个内存描述是否正确
	if (adsp[spnr-1].ap_flgs != ADRSPCE_NOT || \
		adsp[spnr-1].ap_devtype != DEV_TYPE_NOT || \
		adsp[spnr-1].ap_adrstart != 0 || \
		adsp[spnr-1].ap_adrend != 0)
	{
	printfk("结尾错误\n\r");
		return FALSE;
	}

	for (uint_t i=0; i<spnr; i++)
	{
		if (adsp[i].ap_adrend < adsp[i].ap_adrstart)
		{
			printfk("err at %d end=%u < start=%u\n\r", i, adsp[i].ap_adrend, adsp[i].ap_adrstart);
			return FALSE;
		}
		if (adsp[i].ap_flgs == ADRSPCE_SDRAM)
		{
			sdramsts = TRUE;
			// 判断内核是否在SDRAM中
			if (mahp->mh_kerinramstart >= adsp[i].ap_adrstart && \
				mahp->mh_kerinramend < adsp[i].ap_adrend)
			{
				kerinmmsts = TRUE;
			}
		}
	}

	if (sdramsts && kerinmmsts)
	{
		return TRUE;
	}
	printfk(" err .sdramsts=%d,kerinmmsts=%d", sdramsts, kerinmmsts);
	return FALSE;
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
	mahp->mh_mmapdscnr=mindx;
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


void init_phymem(void)
{
	phymem_t_init(&osphymem);
	pmmlist_init(&osmach, &osphymem);
}

void phymem_t_init(phymem_t *initp)
{
	list_init(&initp->pmm_list);
	hal_spinlock_init(&initp->pmm_lock);
	alcfrelst_t_init(&initp->pmm_sz_lsth[0], BLK128KB_BITL);
	alcfrelst_t_init(&initp->pmm_sz_lsth[1], BLK256KB_BITL);
	alcfrelst_t_init(&initp->pmm_sz_lsth[2], BLK512KB_BITL);
	alcfrelst_t_init(&initp->pmm_sz_lsth[3], BLK1MB_BITL);
	alcfrelst_t_init(&initp->pmm_sz_lsth[4], BLK2MB_BITL);
	alcfrelst_t_init(&initp->pmm_sz_lsth[5], BLK4MB_BITL);
}


void alcfrelst_t_init(alcfrelst_t *initp, size_t bsz)
{
	hal_spinlock_init(&initp->afl_lock);
	initp->afl_sz = bsz;

	list_init(&initp->afl_fulllsth);
	list_init(&initp->afl_emtplsth);
	list_init(&initp->afl_fuemlsth);
}


void pmmlist_init(mach_t *mahp, phymem_t *pmp)
{
	mmapdsc_t *mapp = mahp->mh_mmapdscadr;
	uint_t mapnr = mahp->mh_mmapdscnr;

	for (uint_t mr=0; mr<mapnr; mr++)
	{
		mapdsc_addto_memlst(&pmp->pmm_sz_lsth[BLKSZ_HEAD_MAX-1], &mapp[mr], ADDT_EMTP_FLG);
	}
}

void mapdsc_addto_memlst(alcfrelst_t *aflp, mmapdsc_t *mapp, uint_t atflg)
{
	switch (atflg)
	{
		case ADDT_EMTP_FLG:
			list_add_tail(&mapp->map_list, &aflp->afl_emtplsth);
			break;
		case ADDT_FUEM_FLG:
			list_add_tail(&mapp->map_list, &aflp->afl_fuemlsth);
			break;
		case ADDT_FULL_FLG:
			list_add_tail(&mapp->map_list, &aflp->afl_fulllsth);
			break;
	}
}

void onmmapdsc_inkrlram(mach_t *mahp, phymem_t *pmp)
{
	mmapdsc_t*	mapp = mahp->mh_mmapdscadr;
	adr_t 	ker_r_s = mahp->mh_kerinramstart, ker_r_e = mahp->mh_kerinramend;

	if (ker_r_e-ker_r_s > BLK128KB_SIZE || ker_r_s < mapp[0].map_phyadr)
	{
		hal_sysdie("onmmapdsc_inkrlramm err");
	}

	u32_t	cut=1;
	u32_t 	flg = MAP_FLAGES_VAL(0, MAPF_ACSZ_128KB, MAPF_SZ_4MB);

	mapp[0].map_allcount = cut;
	mapp[0].map_flg = flg;
	mapdsc_addto_memlst(&pmp->pmm_sz_lsth[0], &mapp[0], ADDT_FUEM_FLG);
	return;
}
