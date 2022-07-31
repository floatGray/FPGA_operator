#define _HANDLE_GLOBLE

#include "include.h"

void handle_pic(void) {
	location.up = 0;
	location.down = YRES - 1;
	location.left = 0;
	location.right = XRES - 1;

	PROCESS_TRACE("delete trace files", "done");

	BZ_CLR("bz.bmp");
	BZ_CLR("bz_step_1.bmp");
	BZ_CLR("bz_step_2.bmp");
	BZ_CLR("bz_step_3.bmp");
	BZ_CLR("bz_step_4.bmp");
	BZ_CLR("bz_step_5.bmp");
	BZ_CLR("bz_char_0.bmp");
	BZ_CLR("bz_char_1.bmp");
	BZ_CLR("bz_char_2.bmp");
	BZ_CLR("bz_char_3.bmp");
	BZ_CLR("bz_char_4.bmp");
	BZ_CLR("bz_char_5.bmp");
	BZ_CLR("bz_char_6.bmp");
	BZ_CLR("bz_char_7.bmp");
	BZ_CLR("bz_char_8.bmp");
	BZ_CLR("bz_char_9.bmp");
	BZ_CLR("resize_bz_char_0.bmp");
	BZ_CLR("resize_bz_char_1.bmp");
	BZ_CLR("resize_bz_char_2.bmp");
	BZ_CLR("resize_bz_char_3.bmp");
	BZ_CLR("resize_bz_char_4.bmp");
	BZ_CLR("resize_bz_char_5.bmp");
	BZ_CLR("resize_bz_char_6.bmp");
	BZ_CLR("resize_bz_char_7.bmp");
	BZ_CLR("resize_bz_char_8.bmp");
	BZ_CLR("resize_bz_char_9.bmp");

	binaryzation_pic();
	locate_license();
	resize();
}

void binaryzation_pic(void) {
	int i, j;

	for (i = 0; i < YRES; i++) {
		for (j = 0; j < XRES; j++) {
			if ((data_pic[i][j * 4 + 1]) > 112)
				bz_pic[i][j] = 0xff;
			else
				bz_pic[i][j] = 0;
		}
	}
	BZ_TRACE("bz.bmp", location.up, location.down, location.left,
			location.right, bz_pic, XRES);
	PROCESS_TRACE("binaryzation_pic", "done");
}

void locate_license(void) {
	int i, j;
	int cnt = 0;
	char filename[50];

	INFO_TRACE("locate vehicle LPR in the binaryzation data");

	//水平扫描至上下非零边界
	//上边沿
	for (i = 0; i < YRES; i++) {
		for (j = 0; j < XRES; j++) {
			if (!bz_pic[i][j])
				cnt++;
		}
		if (cnt > XRES / 4)
			cnt = 0;
		else {
			location.up = i;
			break;
		}
	}
	//下边沿
	cnt = 0;
	for (i = YRES - 1; i > location.up; i--) {
		for (j = 0; j < XRES; j++) {
			if (!bz_pic[i][j])
				cnt++;
		}
		if (cnt > XRES / 4)
			cnt = 0;
		else {
			location.down = i;
			break;
		}
	}
	//左右扫描至非零边界
	//左边沿
	cnt = 0;
	for (i = 0; i < XRES; i++) {
		for (j = location.up; j < location.down; j++) {
			if (!bz_pic[j][i])
				cnt++;
		}
		if (cnt > (location.down - location.up) / 4)
			cnt = 0;
		else {
			location.left = i;
			break;
		}
	}
	//右边界
	cnt = 0;
	for (i = XRES - 1; i > location.left; i--) {
		for (j = location.up; j < location.down; j++) {
			if (!bz_pic[j][i])
				cnt++;
		}
		if (cnt > (location.down - location.up) / 4)
			cnt = 0;
		else {
			location.right = i;
			break;
		}
	}

	BZ_TRACE("bz_step_1.bmp", location.up, location.down, location.left,
			location.right, bz_pic, XRES);
	INFO_TRACE("license's step 1 location: up: %d, down: %d, left: %d, right: %d",
			location.up, location.down, location.left, location.right);

	//上下扫描至车牌顶空白
	//上边沿
	cnt = 0;
	for (i = location.up; i < location.down; i++) {
		for (j = location.left; j < location.right; j++) {
			if (!bz_pic[i][j])
				cnt++;
		}
		if (cnt > (location.right - location.left) * 9 / 10) {
			location.up = i;
			break;
		} else
			cnt = 0;
	}
	//下边沿
	cnt = 0;
	for (i = location.down; i > location.up; i--) {
		for (j = location.left; j < location.right; j++) {
			if (!bz_pic[i][j])
				cnt++;
		}
		if (cnt > (location.right - location.left) * 9 / 10) {
			location.down = i;
			break;
		} else
			cnt = 0;
	}

	BZ_TRACE("bz_step_2.bmp", location.up, location.down, location.left,
			location.right, bz_pic, XRES);
	INFO_TRACE("license's step 2 location: up: %d, down: %d, left: %d, right: %d",
			location.up, location.down, location.left, location.right);

	//上下扫描至字符顶
	//上边沿
	cnt = 0;
	for (i = location.up; i < location.down; i++) {
		for (j = location.left; j < location.right; j++) {
			if (bz_pic[i][j])
				cnt++;
		}
		if (cnt > (location.right - location.left) / 8) {
			location.up = i;
			break;
		} else
			cnt = 0;
	}
	//下边沿
	cnt = 0;
	for (i = location.down; i > location.up; i--) {
		for (j = location.left; j < location.right; j++) {
			if (bz_pic[i][j])
				cnt++;
		}
		if (cnt > (location.right - location.left) / 8) {
			location.down = i;
			break;
		} else
			cnt = 0;
	}

	BZ_TRACE("bz_step_3.bmp", location.up, location.down, location.left,
			location.right, bz_pic, XRES);
	INFO_TRACE("license's step 3 location: up: %d, down: %d, left: %d, right: %d",
			location.up, location.down, location.left, location.right);

	//左右扫描至车牌空白
	//左边沿
	cnt = 0;
	for (i = location.left; i < location.right; i++) {
		for (j = location.up; j < location.down; j++) {
			if (!bz_pic[j][i])
				cnt++;
		}
		if (cnt > (location.down - location.up) - 5) {
			location.left = i;
			break;
		} else
			cnt = 0;
	}
	//右边沿
	cnt = 0;
	for (i = location.right; i > location.left; i--) {
		for (j = location.up; j < location.down; j++) {
			if (!bz_pic[j][i])
				cnt++;
		}
		if (cnt > (location.down - location.up) - 5) {
			location.right = i;
			break;
		} else
			cnt = 0;
	}

	BZ_TRACE("bz_step_4.bmp", location.up, location.down, location.left,
			location.right, bz_pic, XRES);
	INFO_TRACE("license's step 4 location: up: %d, down: %d, left: %d, right: %d",
			location.up, location.down, location.left, location.right);

	//左右扫描至车牌字符
	//左边沿
	cnt = 0;
	for (i = location.left + 3; i < location.right; i++) {
		for (j = location.up; j < location.down; j++) {
			if (bz_pic[j][i])
				cnt++;
		}
		if (cnt > (location.down - location.up) / 8) {
			location.left = i;
			break;
		} else
			cnt = 0;
	}
	//右边沿
	cnt = 0;
	for (i = location.right - 3; i > location.left; i--) {
		for (j = location.up; j < location.down; j++) {
			if (bz_pic[j][i])
				cnt++;
		}
		if (cnt > (location.down - location.up) / 8) {
			location.right = i;
			break;
		} else
			cnt = 0;
	}

	BZ_TRACE("bz_step_5.bmp", location.up, location.down, location.left,
			location.right, bz_pic, XRES);
	INFO_TRACE("license's step 5 location: up: %d, down: %d, left: %d, right: %d",
			location.up, location.down, location.left, location.right);

	//确定字符边界
	int num = EDGE_COUNT - 1;
	int flag = 0;
	cnt = 0;
	memset(edge, 0, EDGE_COUNT);
	for (i = location.right; i > location.left; i--) {
		for (j = location.up; j < location.down; j++) {
			if (bz_pic[j][i])
				cnt++;
		}
		if (flag == 0) {
			if (cnt > 5) {
				INFO_TRACE("hit right point -%d-: %d", num, i);
				edge[num--] = i;
				cnt = 0;
				flag = 1;
				/**
				 * @Date: 2021-04-09 12:58:36
				 * @Author: Ciao
				 * new add
				 */
				//fix chuan
				if(edge[num+1] < 65 + location.left)
				{
					INFO_TRACE("hit left point -%d-: %d", num, location.left);
					edge[num--] = location.left;
					cnt = 0;
					flag = 0;
					i = location.left;
				}

			} else
				cnt = 0;
		} else {
			if (cnt < 5) {
				/**
				 * @Date: 2021-04-13 19:48:27
				 * @Author: Ciao
				 * new add
				 */
				if(edge[num+1] - i > 120)
				{
					edge[num] = (edge[num+1] + i)/2 + 3;
					num--;
					edge[num] = (edge[num+2] + i)/2 - 3;
					num--;
				}
				INFO_TRACE("hit left point -%d-: %d", num, i);
				edge[num--] = i;
				cnt = 0;
				flag = 0;
			} else
				cnt = 0;
		}
		if (num == 0)
			break;
	}
	edge[0] = location.left;

	//去掉0值坐标
	j = location.left;
	for (i = 1; i < EDGE_COUNT; i++) {
		if (!edge[i])
			continue;
		else {
			edge[i - 1] = j;
			j = edge[i];
		}
	}

	for (i = 0; i < EDGE_COUNT / 2; i++) {
		if (!edge[i * 2] || !edge[i * 2 + 1])
			continue;
		memset(filename, 0, 50);
		sprintf(filename, "bz_char_%d.bmp", i);
		BZ_TRACE(filename, location.up, location.down, edge[i * 2],
				edge[i * 2 + 1], bz_pic, XRES);
		INFO_TRACE(
				"license's char %d location: up: %d, down: %d, left: %d, right: %d",
				i, location.up, location.down, edge[i * 2], edge[i * 2 + 1]);
	}
}
/**
 * @Date: 2021-04-17 20:07:29
 * @Author: Ciao
 * new add
 */
int onenum = -1;

void resize(void)
{
	int ver = (location.down - location.up) / YRESIZE + 1;
	int vdiff = YRESIZE - ((location.down - location.up) / ver);
	int hor;
	int hordiff;
	char filename[50];	
	int i, j;
	int m;

	INFO_TRACE("resize characters to %d * %d binaryzation data", XRESIZE, YRESIZE);
	
	chrnum = 0;
	for (m = 1; m < EDGE_COUNT / 2; m++) {
		if ((!edge[m * 2] || !edge[m * 2 + 1]) || (edge[m*2]<edge[m*2-1]))
			continue;
		/**
		 * @Date: 2021-04-09 12:48:48
		 * @Author: Ciao
		 * new add
		 */
		//clear point & fix 1
		if(edge[m * 2 + 1] - edge[m * 2] < 15)
			continue;
		if(edge[m * 2 + 1] - edge[m * 2] < 30)
			onenum = chrnum;
		INFO_TRACE("resize No.%d. left: %d; right: %d", chrnum, edge[m * 2], edge[m * 2 + 1]);
		hor = (edge[m * 2 + 1] - edge[m * 2]) / XRESIZE + 1;
		hordiff = XRESIZE - (edge[m * 2 + 1] - edge[m * 2]) / hor;
		for (i = 0; i < YRESIZE; i++)
		{
			for (j = 0; j < XRESIZE; j++)
			{
				if ((i < (vdiff / 2)) || (i > (YRESIZE - (vdiff / 2))) || (j < (hordiff / 2)) || (j > (XRESIZE - (hordiff / 2)))){
					plate.unit[chrnum][i][j] = 0.0;
#ifdef BZ_DEBUG
					plate.bz_unit[chrnum][i][j] = 0;
#endif
				}
				else{
					plate.unit[chrnum][i][j] = !bz_pic[location.up + (i - vdiff / 2) * ver][edge[m * 2] + (j - hordiff / 2) * hor] ? 0.0 : 1.0;
#ifdef BZ_DEBUG
					plate.bz_unit[chrnum][i][j] = plate.unit[chrnum][i][j] == 0.0 ? 0 : 0xff;
#endif
				}
			}
		}
		memset(filename, 0, 50);
		sprintf(filename, "resize_bz_char_%d.bmp", m);
		BZ_TRACE(filename, 0, YRESIZE - 1, 0, XRESIZE - 1, plate.bz_unit[chrnum], XRESIZE);
		chrnum++;
	}
}

#undef _HANDLE_GLOBLE
