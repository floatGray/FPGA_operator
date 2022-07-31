#define _CHR_GLOBLE

#include "include.h"

//根据infer.c返回的值进行查找、打印相应的汉字或字符
void chrfind(int seq, int index)
{
	if(seq == 0){
		switch (index)
		{
			case 0:strcpy(chrret, "川"); break;
			case 1:strcpy(chrret, "鄂"); break;
			case 2:strcpy(chrret, "赣"); break;
			case 3:strcpy(chrret, "甘"); break;
			case 4:strcpy(chrret, "贵"); break;
			case 5:strcpy(chrret, "桂"); break;
			case 6:strcpy(chrret, "黑"); break;
			case 7:strcpy(chrret, "沪"); break;
			case 8:strcpy(chrret, "冀"); break;
			case 9:strcpy(chrret, "津"); break;
			case 10:strcpy(chrret, "京"); break;
			case 11:strcpy(chrret, "吉"); break;
			case 12:strcpy(chrret, "辽"); break;
			case 13:strcpy(chrret, "鲁"); break;
			case 14:strcpy(chrret, "蒙"); break;
			case 15:strcpy(chrret, "闽"); break;
			case 16:strcpy(chrret, "宁"); break;
			case 17:strcpy(chrret, "青"); break;
			case 18:strcpy(chrret, "琼"); break;
			case 19:strcpy(chrret, "陕"); break;
			case 20:strcpy(chrret, "苏"); break;
			case 21:strcpy(chrret, "晋"); break;
			case 22:strcpy(chrret, "皖"); break;
			case 23:strcpy(chrret, "湘"); break;
			case 24:strcpy(chrret, "新"); break;
			case 25:strcpy(chrret, "豫"); break;
			case 26:strcpy(chrret, "渝"); break;
			case 27:strcpy(chrret, "粤"); break;
			case 28:strcpy(chrret, "云"); break;
			case 29:strcpy(chrret, "藏"); break;
			case 30:strcpy(chrret, "浙"); break;
		}
	}
	else {
		switch (index)
		{
			case 0:strcpy(chrret, "0"); break;
			case 1:strcpy(chrret, "1"); break;
			case 2:strcpy(chrret, "2"); break;
			case 3:strcpy(chrret, "3"); break;
			case 4:strcpy(chrret, "4"); break;
			case 5:strcpy(chrret, "5"); break;
			case 6:strcpy(chrret, "6"); break;
			case 7:strcpy(chrret, "7"); break;
			case 8:strcpy(chrret, "8"); break;
			case 9:strcpy(chrret, "9"); break;
			case 10:strcpy(chrret, "A"); break;
			case 11:strcpy(chrret, "B"); break;
			case 12:strcpy(chrret, "C"); break;
			case 13:strcpy(chrret, "D"); break;
			case 14:strcpy(chrret, "E"); break;
			case 15:strcpy(chrret, "F"); break;
			case 16:strcpy(chrret, "G"); break;
			case 17:strcpy(chrret, "H"); break;
			case 18:strcpy(chrret, "J"); break;
			case 19:strcpy(chrret, "K"); break;
			case 20:strcpy(chrret, "L"); break;
			case 21:strcpy(chrret, "M"); break;
			case 22:strcpy(chrret, "N"); break;
			case 23:strcpy(chrret, "P"); break;
			case 24:strcpy(chrret, "Q"); break;
			case 25:strcpy(chrret, "R"); break;
			case 26:strcpy(chrret, "S"); break;
			case 27:strcpy(chrret, "T"); break;
			case 28:strcpy(chrret, "U"); break;
			case 29:strcpy(chrret, "V"); break;
			case 30:strcpy(chrret, "W"); break;
			case 31:strcpy(chrret, "X"); break;
			case 32:strcpy(chrret, "Y"); break;
			case 33:strcpy(chrret, "Z"); break;
		}
	}
}

#undef _CHR_GLOBLE
