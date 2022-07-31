#define _HLS_GLOBLE

#include "include.h"

void accconvolution(int width, int height, int outdir, int accmulate) {
	int ctl = 0;
	ctl |= width;
	ctl |= height << 8;
	ctl |= outdir << 17;
	ctl |= accmulate << 18;

	*(acc_ctl_virtual_base) = ctl; //参数
	*(acc_ctl_virtual_base + 1) = 0x8; //控制

	while (((*(acc_ctl_virtual_base + 1) >> 3) & 0x1)){
	}
}

void accbiasrelu(int width, int height, int indir, int outdir) {
	int ctl = 0;
	ctl |= width;
	ctl |= height << 8;
	ctl |= indir << 16;
	ctl |= outdir << 17;

	*(acc_ctl_virtual_base) = ctl; //参数
	*(acc_ctl_virtual_base + 1) = 0x1; //控制

	while ((*(acc_ctl_virtual_base + 1) & 0x1)){
	}
}

void accpool(int width, int height, int indir, int outdir) {
	int ctl = 0;

	ctl |= width;
	ctl |= height << 8;
	ctl |= indir << 16;
	ctl |= outdir << 17;

	*(acc_ctl_virtual_base) = ctl; //参数
	*(acc_ctl_virtual_base + 1) = 0x2; //控制

	while (((*(acc_ctl_virtual_base + 1) >> 1) & 0x1)) {
	}
}

void accfullconnect(void) {
	int ctl = 0;

	*(acc_ctl_virtual_base) = ctl; //参数
	*(acc_ctl_virtual_base + 1) = 0x4; //控制

	while (((*(acc_ctl_virtual_base + 1) >> 2) & 0x1)) {
	}
}

#undef _HLS_GLOBLE
