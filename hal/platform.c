/**************************************
  平台相关的文件:platform.c

  **************************************/
#include "lmosemtypes.h"
#include "lmosemmctrl.h"


void init_paltform()
{
		s3c2440mmu_init();
		s3c2440vector_init();
		init_halmach();

		return;
}

LKHEAD_T void s3c2440mmu_init()
{
	uint_t	paddr = 0;
	u32_t	pgdsc = 0;
	u32_t*	pgdiradr = (u32_t *)PAGE_TLB_DIR;	
	for (uint_t i=0; i<PAGE_TLB_SIZE; i++)
	{
		pgdsc = paddr | PTE_SECT_AP | PTE_SECT_DOMAIN | PTE_SECT_NOCW | PTE_SECT_BIT;
		pgdiradr[i] = pgdsc;
		paddr += 0x100000;

	}
	pgdiradr[0] = SDRAM_MAPVECTPHY_ADDR | PTE_SECT_AP | PTE_SECT_DOMAIN | PTE_SECT_NOCW | PTE_SECT_BIT; 

	s3c2440mmu_set_tblbass(PAGE_TLB_DIR);
	//设置MMU中打16个域打权限时管理员
	s3c2440mmu_set_domain(~0);
	s3c2440mmu_invalid_dicache();
	hal_disable_cache();
	s3c2440mmu_enable();

	return;


}

LKHEAD_T void s3c2440mmu_set_tblbass(u32_t tblbphyaddr)
{
		__asm__ __volatile__(
			"mcr p15,0,%[tbass],c2,c0,0 \n\t"
			:
	//分配一个通用寄存器，在其中写入tblbphyaddr的内容。由Gcc完成该动作
			:[tbass] "r" (tblbphyaddr)
			: "cc", "memory"
		);
		return;
}
LKHEAD_T void s3c2440mmu_set_domain(u32_t domain)
{
		__asm__ __volatile__(
			"mcr p15,0,%[domval],c3,c0,0 \n\t"
			:
			:[domval]"r"(domain)
			:"cc","memory"
		);
		return;
}
LKHEAD_T void s3c2440mmu_invalid_dicache()
{
		__asm__ __volatile__(
			"mov	r0, #0 \n\t"
			//使数据和指令cache无效
			"mcr p15,0,r0,c7,c7,0 \n\t"
			//清空写缓存区
			"mcr p15,0,r0,c7,c10,4 \n\t"
			//使指令TLB，数据TLB无效
			"mcr p15,0,r0,c8,c7,0 \n\t"
			:
			:
			:"cc","memory","r0"
		);
		return;
}

LKHEAD_T void s3c2440mmu_enable()
{
		__asm__ __volatile__(
			//r0=c1
			"mrc p15,0,r0,c1,c0,0	\n\t"
			//r0.0=1
			"orr r0,#1		\n\t"
			"mcr p15,0,r0,c1,c0,0	\n\t"
			"nop	\n\t"
			"nop	\n\t"
			"nop	\n\t"
			"nop	\n\t"
			"nop	\n\t"
			"nop	\n\t"
			"nop	\n\t"
			:
			:
			: "r0","cc","memory"
		);
}


LKHEAD_T void s3c2440vector_init()
{
	s3c2440vector_copy();
	return;
}

LKHEAD_T void s3c2440vector_copy()
{
	u32_t* src_t = (u32_t *)(&__end_lmosem_hal_vector);
	u32_t* src = (u32_t *)(&vector); //vector.s中定义的符号
	u32_t* det = (u32_t *)(CPU_VECTOR_VIRADR);

	for (u32_t i=0; i<4096; i++)
	{
		det[i] = 0;
	}

	for (;src<src_t; src++, det++)
	{
		*det = *src;
	}

	return;
}
