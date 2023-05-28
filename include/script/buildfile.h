#ifndef BUILDFILE_H
#define BUILDFILE_H

#define BUILD_BOOT_OBJS
#define BUILD_HALY_OBJS init.o lmosemhal_start.o vector.o interrupt.o halinit.o halmmu.o platform.o haluart.o cpuctrl.o _divsi3.o _modsi3.o _udivsi3.o \
						_umodsi3.o div0.o printfk.o halglobal.o halmach.o halmm.o halintupt.o

#define BUILD_KRNL_OBJS 
#define BUILD_MEMY_OBJS
#define BUILD_FSYS_OBJS
#define BUILD_DRIV_OBJS 
#define BUILD_LIBS_OBJS 
#define BUILD_TASK_OBJS  
#define BUILD_LINK_OBJS BUILD_BOOT_OBJS BUILD_HALY_OBJS


#define LINKR_IPUT_FILE BUILD_LINK_OBJS
#define LINKR_OPUT_FILE lmosemkrnl.elf
#define KERNL_ELFF_FILE LINKR_OPUT_FILE
#define KERNL_BINF_FILE lmosemkrnl.bin
                          
#endif // BUILDFILE_H
