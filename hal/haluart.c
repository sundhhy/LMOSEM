/*******************************************
  uart0驱动文件haluart.c
  *****************************************/
#include "lmosemtypes.h"
#include "lmosemmctrl.h"


void init_haluart()
{
	init_uart0();
	//hal_uart0_putc('s');
	//hal_uart0_putc('d');
	//hal_uart0_putc('h');
	uint_t	val1 = 12;
	char	*test_str = "test printf";
	printfk("vald is %d,valx is %x, str is %s",
		val1, val1, test_str);
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

drvstus_t hal_uart_write(uint_t uart, void *buf, uint_t len)
{
	char_t *p = buf;
	cpuflg_t cpuflg;

	hal_disablefiq_savecpuflg(&cpuflg);

	while (*p)
	{
			if (uart_send_char(uart, *p) == DFCERRSTUS)
			{
					hal_sysdie("uart err");
					return DFCERRSTUS;
			}
			p++;

	}
	hal_enableirqfiq_restcpuflg(&cpuflg);
	return DFCOKSTUS;
}


drvstus_t uart_send_char(uint_t uart, char_t ch)
{
	uint_t	time = 0;
	if (uart != 0)
		return DFCERRSTUS;

	while (!(hal_io32_read(UTRSTAT0_R) & 4))
	{
		if (time>0x1000000)
		{
			return DFCERRSTUS;
		}
		time ++;
	}

	hal_io32_write(UTXH0_R, ch);

	return DFCOKSTUS;
}
