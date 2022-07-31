#define _TOOLS_GLOBLE

#include "include.h"

void printf_clock(void){
	clock_gettime(CLOCK_MONOTONIC, &trace_ts);
	printf("<%6d:%-11d>     ",trace_ts.tv_sec,trace_ts.tv_nsec);
}

float hex2float(int weight) {
	float f;
	memcpy(&f, &weight, sizeof(float));
	return f;
}

int float2hex(float weight) {
	int f;
	memcpy(&f, &weight, sizeof(float));
	return f;
}

//读取参数文件的函数
void parameter_init() {
	FILE* f_para;
	int weighthex;
	int i, j, k, m;

	f_para = fopen(PARA_FILEPATH, "r+");

	//---------------------------
	//ChrInfer 参数
	//---------------------------
	//读取第一层卷积窗
	for (i = 0; i < L1CHAN; i++) {
		for (j = 0; j < ConvSize; j++) {
			fscanf(f_para, "%x", &weighthex);
			para.chrlayer1weight[i][j] = hex2float(weighthex);
		}
	}

	//读取第二层卷积窗
	for (i = 0; i < L2CHAN; i++) {
		for (j = 0; j < L1CHAN; j++) {
			for (k = 0; k < ConvSize; k++) {
				fscanf(f_para, "%x", &weighthex);
				para.chrlayer2weight[i][j][k] = hex2float(weighthex);
			}
		}
	}

	//读取第三层卷积窗
	for (i = 0; i < L3CHAN; i++) {
		for (j = 0; j < L2CHAN; j++) {
			for (k = 0; k < ConvSize; k++) {
				fscanf(f_para, "%x", &weighthex);
				para.chrlayer3weight[i][j][k] = hex2float(weighthex);
			}
		}
	}

	//读取第一全连接层权重
	for (i = 0; i < NER; i++) {
		for (j = 0; j < L3CHAN; j++) {
			for (k = 0; k < 6; k++) {
				for (m = 0; m < 3; m++) {
					fscanf(f_para, "%x", &weighthex);
					para.chrfull1weight[i][j][k][m] = hex2float(weighthex);
				}
			}
		}
	}

	//读取第二全连接层权重
	for (i = 0; i < ChrCls; i++) {
		for (j = 0; j < NER; j++) {
			fscanf(f_para, "%x", &weighthex);
			para.chrfull2weight[i][j] = hex2float(weighthex);
		}
	}

	//读取第一卷积层偏置值
	for (i = 0; i < L1CHAN; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chrlayer1bias[i] = hex2float(weighthex);
	}

	//读取第二卷积层偏置值
	for (i = 0; i < L2CHAN; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chrlayer2bias[i] = hex2float(weighthex);
	}

	//读取第三卷积层偏置值
	for (i = 0; i < L3CHAN; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chrlayer3bias[i] = hex2float(weighthex);
	}

	//读取第一全连接层偏置值
	for (i = 0; i < NER; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chrnerbias[i] = hex2float(weighthex);
	}

	//读取第二全连接层偏置值
	for (i = 0; i < ChrCls; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chrprobias[i] = hex2float(weighthex);
	}

	//---------------------------
	//ChnInfer 参数
	//---------------------------
	//读取第一层卷积窗
	for (i = 0; i < L1CHAN; i++) {
		for (j = 0; j < ConvSize; j++) {
			fscanf(f_para, "%x", &weighthex);
			para.chnlayer1weight[i][j] = hex2float(weighthex);
		}
	}

	//读取第二层卷积窗
	for (i = 0; i < L2CHAN; i++) {
		for (j = 0; j < L1CHAN; j++) {
			for (k = 0; k < ConvSize; k++) {
				fscanf(f_para, "%x", &weighthex);
				para.chnlayer2weight[i][j][k] = hex2float(weighthex);
			}
		}
	}

	//读取第三层卷积窗
	for (i = 0; i < L3CHAN; i++) {
		for (j = 0; j < L2CHAN; j++) {
			for (k = 0; k < ConvSize; k++) {
				fscanf(f_para, "%x", &weighthex);
				para.chnlayer3weight[i][j][k] = hex2float(weighthex);
			}
		}
	}

	//读取第一全连接层权重
	for (i = 0; i < NER; i++) {
		for (j = 0; j < L3CHAN; j++) {
			for (k = 0; k < 6; k++) {
				for (m = 0; m < 3; m++) {
					fscanf(f_para, "%x", &weighthex);
					para.chnfull1weight[i][j][k][m] = hex2float(weighthex);
				}
			}
		}
	}

	//读取第一全连接层权重
	for (i = 0; i < ChnCls; i++) {
		for (j = 0; j < NER; j++) {
			fscanf(f_para, "%x", &weighthex);
			para.chnfull2weight[i][j] = hex2float(weighthex);
		}
	}

	//读取第一卷积层偏置值
	for (i = 0; i < L1CHAN; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chnlayer1bias[i] = hex2float(weighthex);

	}

	//读取第二卷积层偏置值
	for (i = 0; i < L2CHAN; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chnlayer2bias[i] = hex2float(weighthex);
	}

	//读取第三卷积层偏置值
	for (i = 0; i < L3CHAN; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chnlayer3bias[i] = hex2float(weighthex);
	}

	//读取第一全连接层偏置值
	for (i = 0; i < NER; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chnnerbias[i] = hex2float(weighthex);
	}

	//读取第二全连接层偏置值
	for (i = 0; i < ChnCls; i++) {
		fscanf(f_para, "%x", &weighthex);
		para.chnprobias[i] = hex2float(weighthex);
	}

	fclose(f_para);

	PROCESS_TRACE("parameter_init", "done");
}

#undef _TOOLS_GLOBLE
