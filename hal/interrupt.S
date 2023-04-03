/***********************************************
  	中断异常入口文件interrupt.S
	************************************/

.section .lmosem_hal.intvect,"ax"
.code 32
.align 4
.global hal_undefins
.global hal_swi
.global hal_prefabt
.global hal_dataabt
.global hal_bugdie
.global hal_irq
.global hal_frq

hal_undefins:
	b .
hal_swi:
	b .
hal_prefabt:
	b .
hal_dataabt:
	b .
hal_bugdie:
	b .
hal_irq:
	b .
hal_frq:
	b .
