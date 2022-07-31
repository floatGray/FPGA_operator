#define _FPGA_GLOBLE

#include "include.h"

int h2f_lw_axi_master_init(void** virtual_base) {
	int fd;
	void* periph_virtual_base;

	fd = open("/dev/mem", (O_RDWR | O_SYNC));
	assert(fd != -1);
	periph_virtual_base = (unsigned int*) mmap(NULL, HW_REGS_SPAN,
			(PROT_READ | PROT_WRITE),
			MAP_SHARED, fd, HW_REGS_BASE);
	assert(periph_virtual_base != MAP_FAILED);

#ifndef INOTIFY_ENABLE
	h2p_dvp_capture_addr = (periph_virtual_base
			+ ((unsigned long) (ALT_LWFPGASLVS_OFST + DVP_CAP_0_BASE)
					& (unsigned long) (HW_REGS_MASK)));
#else
	fbdev.fb_vbase = (periph_virtual_base
			+ ((unsigned long) (ALT_LWFPGASLVS_OFST + ALT_VIP_CL_VFB_0_BASE)
					& (unsigned long) (HW_REGS_MASK)));
#endif

	*virtual_base = periph_virtual_base;

	PROCESS_TRACE("h2f_lw_axi_master_init", "done");

	return fd;
}

int h2f_axi_master_init(void** virtual_base) {
	int fd;
	void* periph_virtual_base;

	fd = open("/dev/mem", ( O_RDWR | O_SYNC));
	assert(fd != -1);
	periph_virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE),
	MAP_SHARED, fd, 0xc0000000);
	assert(periph_virtual_base != MAP_FAILED);

	//acc_ctl
	acc_ctl_virtual_base = periph_virtual_base
			+ ((unsigned long) ( ALT_LWFPGASLVS_OFST + ACCSYSTEM_0_CTL_BASE)
					& (unsigned long) ( HW_REGS_MASK));
	//acc_data
	acc_data_virtual_base = periph_virtual_base
			+ ((unsigned long) ( ALT_LWFPGASLVS_OFST + ACCSYSTEM_0_DATA_BASE)
					& (unsigned long) ( HW_REGS_MASK));

	*virtual_base = periph_virtual_base;

	PROCESS_TRACE("h2f_axi_master_init", "done");

	return fd;
}

#undef _FPGA_GLOBLE
