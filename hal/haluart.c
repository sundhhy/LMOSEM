/*******************************************
  uart0驱动文件haluart.c
  *****************************************/
#include "lmosemtypes.h"
#include "lmosemmctrl.h"


void init_haluart()
{
	init_uart0();
	hal_uart0_putc('s');
	hal_uart0_putc('d');
	hal_uart0_putc('h');
	return;
}

void init_uart0()
{
	hal_io32_write(ULCON0_R, 3); //8 1 None
	hal_io32_write(UCON0_R, 5); //中断模式
	hal_io32_write(UFCON0_R, 0); //disable FIFO
	hal_io32_write(UMCON0_R, 0);
	hal_io32_write(UBRDIV0_R, 26);	//115200
}

void hal_uart0_putc(char_t c)
{
	while (!(hal_io32_read(UTRSTAT0_R)&4));

	hal_io32_write(UTXH0_R,c);
	return;
}
