#define _IMAGE_GLOBLE

#include "include.h"

int read_pic(char* file) {
	struct bitmapfileheader_desc fileheader;
	struct bitmapinfoheader_desc infoheader;
	unsigned short filetype;
	unsigned int offset;
	char path_pic[50]; //路径拼接
	int ret;
	int i;

	//读取bmp图片到数组
	memset(path_pic, 0, 50);
	strcat(path_pic, INOTIFY_PATH);
	strcat(path_pic, file);
	printf("%s", path_pic);
	f_pic = fopen(path_pic, "rw");
	assert(f_pic > 0);
	//读取bmp文件信息
	ret = fread(&filetype, sizeof(unsigned short), 1, f_pic);
	if (!ret) {
		PROCESS_TRACE("read filetype", "failed");
		goto read_pic_out;
	} else
		VALUEx_TRACE("filetype", filetype);
	if (filetype == 0x4d42) {
		ret = fread(&fileheader, sizeof(fileheader), 1, f_pic);
		if (ret < 0) {
			PROCESS_TRACE("read fileheader", "failed");
			goto read_pic_out;
		}
		VALUEx_TRACE("fileheader.bfsize", fileheader.bfsize);
		VALUEx_TRACE("fileheader.bfoffbits", fileheader.bfoffbits);
		offset = fileheader.bfoffbits;
		ret = fread(&infoheader, sizeof(infoheader), 1, f_pic);
		if (ret < 0) {
			PROCESS_TRACE("read infoheader", "failed");
			goto read_pic_out;
		}
		VALUEx_TRACE("infoheader.bisize", infoheader.bisize);
		VALUEd_TRACE("infoheader.bitwidth", infoheader.bitwidth);
		VALUEd_TRACE("infoheader.bitheight", infoheader.bitheight);
		VALUEx_TRACE("infoheader.biplanes", infoheader.biplanes);
		VALUEx_TRACE("infoheader.bibitcount", infoheader.bibitcount);
		VALUEx_TRACE("infoheader.bicompression", infoheader.bicompression);
		VALUEx_TRACE("infoheader.bisizeimage", infoheader.bisizeimage);
		VALUEx_TRACE("infoheader.bixpelspermeter", infoheader.bixpelspermeter);
		VALUEx_TRACE("infoheader.biypelspermeter", infoheader.biypelspermeter);
		VALUEx_TRACE("infoheader.biclrused", infoheader.biclrused);
		VALUEx_TRACE("infoheader.biclrimportant", infoheader.biclrimportant);
		ret = 0;
		if (infoheader.bitwidth != XRES || infoheader.bitheight != YRES) {
			WARNING_TRACE(
					"current bmp is %u * %u, is not supported. please upload %u * %u bmp again",
					infoheader.bitwidth, infoheader.bitheight, XRES, YRES);
			goto read_pic_out;
		}
		if (infoheader.bitheight < 0) {
			WARNING_TRACE("current bmp is inversion, is not supported");
			goto read_pic_out;
		}
		if (infoheader.bibitcount != 32) {
			WARNING_TRACE(
					"current bmp is %d bits per pix, is not supported. please upload 32 bits bmp again",
					infoheader.bibitcount);
			goto read_pic_out;
		}
		if (infoheader.bicompression != 0) {
			WARNING_TRACE("current bmp is compressed, is not supported");
			goto read_pic_out;
		}
		fseek(f_pic, offset + 1, SEEK_SET);
		//填充数组
		//for (i = 0; i < YRES; i++) {
		for (i = YRES - 1; i >= 0; i--) {
			ret = fread(data_pic[i], sizeof(unsigned char), XRES * 4, f_pic);
			assert(ret > 0);
		}
		PROCESS_TRACE("read_pic", "done");
	} else {
		WARNING_TRACE("the file type is %u, is not supported", filetype);
		ret = 0;
		goto read_pic_out;
	}
	goto read_pic_out;
	read_pic_out: fclose(f_pic);
	return ret;
}

void draw_8bit_bmp(char* file, int up, int down, int left, int right, char* buf, int bufwidth) {
	struct bitmapfileheader_desc fileheader;
	struct bitmapinfoheader_desc infoheader;
	unsigned short filetype;
	char path_bz[50];//路径拼接
	int width_calc;//图像宽度调整至4的倍数
	int ret;
	int i, j;

	memset(path_bz, 0, 50);
	strcat(path_bz, BZ_PATH);
	strcat(path_bz, file);
	FILE *f_bz = fopen(path_bz, "w");
	assert(f_bz > 0);
	//图像宽度调整至4的倍数
	width_calc = (right - left + 1 + 3) / 4 * 4;
	//填充bmp头
	filetype = 0x4d42;
	fileheader.bfsize = (down - up + 1) * width_calc + 0x436;
	fileheader.bfreserved1 = 0;
	fileheader.bfreserved2 = 0;
	fileheader.bfoffbits = 0x436;
	infoheader.bisize = 0x28;
	infoheader.bitwidth = width_calc;
	infoheader.bitheight = down - up + 1;
	infoheader.biplanes = 0x1;
	infoheader.bibitcount = 0x8;
	infoheader.bicompression = 0;
	infoheader.bisizeimage = (down - up + 1) * width_calc;
	infoheader.bixpelspermeter = 0;		//0xec3;
	infoheader.biypelspermeter = 0;		//0xec3;
	infoheader.biclrused = 0;
	infoheader.biclrimportant = 0;
	ret = fwrite(&filetype, sizeof(unsigned short), 1, f_bz);
	assert(ret > 0);
	ret = fwrite(&fileheader, sizeof(fileheader), 1, f_bz);
	assert(ret > 0);
	ret = fwrite(&infoheader, sizeof(infoheader), 1, f_bz);
	assert(ret > 0);
	ret = fwrite(&colorpalette, PALETTE_LENGTH, 1, f_bz);
	assert(ret > 0);
	//填充数组
	for (i = up; i <= down; i++) {
		ret = fwrite(buf + (i * bufwidth + left), sizeof(unsigned char),
				right - left + 1, f_bz);
		assert(ret > 0);
		//分辨率扩展
		for (j = 0; j < width_calc - (right - left + 1); j++) {
			ret = fwrite(buf + (i * bufwidth + right), sizeof(unsigned char), 1, f_bz);
			assert(ret > 0);
		}
	}
	fclose(f_bz);
	INFO_TRACE("write bz data to file: %s", path_bz);
}

void clr_bz(char* file) {
	char path_bz[50]; //路径拼接

	memset(path_bz, 0, 50);
	strcat(path_bz, BZ_PATH);
	strcat(path_bz, file);

	remove(path_bz);
}

#undef _IMAGE_GLOBLE
