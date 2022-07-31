#define _GRAPHICS_GLOBLE

#include "include.h"

void fbdev_init(void) {
#ifndef INOTIFY_ENABLE
	fbdev.fd = open("/dev/fb0", O_RDWR);
	assert(fbdev.fd != -1);

	ioctl(fbdev.fd, FBIOGET_FSCREENINFO, &(fbdev.finfo));
	ioctl(fbdev.fd, FBIOGET_VSCREENINFO, &(fbdev.vinfo));

	fbdev.fb_index = 0;

	fbdev.fb_screensize = fbdev.vinfo.xres * fbdev.vinfo.yres
			* fbdev.vinfo.bits_per_pixel / 8;

	fbdev.fb_vmap = (unsigned char *) mmap(NULL, fbdev.finfo.smem_len,
		PROT_READ | PROT_WRITE, MAP_SHARED, fbdev.fd, 0);
	fbdev.fb_currunt_vmap = fbdev.fb_vmap;

	assert(fbdev.fb_vmap != MAP_FAILED);

	PROCESS_TRACE("fbdev_init", "done");
#endif
}

void wait_pan_framebuffer(void){
	while (!(fbdev.fb_vbase[7] & 0x04000000))
		;
}

void set_pan_framebuffer(void){
	fbdev.fb_index = (fbdev.fb_index + 1) % BUFFER_COUNT;
	assert(fbdev.fb_index <= BUFFER_COUNT);
	fbdev.fb_currunt_vmap = (unsigned char*)(fbdev.fb_vmap) + (fbdev.fb_screensize * fbdev.fb_index);
	fbdev.fb_currunt_smem_start = (unsigned char*)(fbdev.finfo.smem_start + (fbdev.fb_screensize * fbdev.fb_index));
}

void pan_framebuffer(void){	
	fbdev.fb_vbase[6] = (unsigned int)(fbdev.fb_currunt_smem_start);
}

#undef _GRAPHICS_GLOBLE
