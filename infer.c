#define _INFER_GLOBLE

#include "include.h"

//  汉字识别流程与字符识别流程几乎一模一样
//  唯一的区别就是最终的结果数组大小不同
/**
 * @Date: 2021-04-17 20:07:42
 * @Author: Ciao
 * new add
 */
extern int onenum;
int chrinfer(int seq) {
	//Seq参数表示从左往右在车牌的第几个字符
	//----------------------------------------------
	//  第一卷积层
	//----------------------------------------------
	//将输入图像加载至FPGA的Map存储器中
	//基地址偏移为0
	//第一层为1通道输入，8通道输出，输入图像为同一个
	//所以只需要首先加载一次图像数据
	//然后每次卷积时每个通道再单独加载权重
	//加载过程中处理Padding，保持输出图像与输入图像大小相同
	//即上下左右补一圈0
	VALUEd_TRACE("chrinfer seq", seq);
	INFO_TRACE("level 1 accconvolution");
	int i, j, channel, outchannel, inchannel, ner;
	//Padding后输入图像大小变为50x26
	for (i = 0; i < 50; i++) {
		for (j = 0; j < 26; j++) {
			//上下补0，即第0行或最后一行，写入Map存储器中的数据全是0
			if ((i == 0) || (i == 49)) {
				*(acc_data_virtual_base + i * 26 + j) = 0.0;
#ifdef HLS_DEBUG
				*(sram_hps + i * 26 + j) = 0.0;
#endif
			} else {
				//左右补0，即第0列或最后一列，写入Map存储器中的数据全是0
				if ((j == 0) || (j == 25)) {
					*(acc_data_virtual_base + i * 26 + j) = 0.0;
#ifdef HLS_DEBUG
					*(sram_hps + i * 26 + j) = 0.0;
#endif
				} else {
					//根据unit参数对字符图像进行选择后写入Map存储器中
					*(acc_data_virtual_base + i * 26 + j) = plate.unit[seq][i - 1][j - 1];
#ifdef HLS_DEBUG
					*(sram_hps + i * 26 + j) = plate.unit[seq][i - 1][j - 1];
#endif
				}
			}
		}
	}

	//第一层开始
	for (channel = 0; channel < L1CHAN; channel++) {
		//卷积
		//加载权重至FPGA Weight存储器中
		//基地址偏移为FPGA_Weight
		for (i = 0; i < ConvSize; i++) {
			*(acc_data_virtual_base + i + FPGA_Weight) = seq == 0 ? para.chnlayer1weight[channel][i] : para.chrlayer1weight[channel][i];
#ifdef HLS_DEBUG
			*(sram_hps + i + FPGA_Weight) = seq == 0 ? para.chnlayer1weight[channel][i] : para.chrlayer1weight[channel][i];
#endif			
		}
		//配置卷积加速器
		//由于输入图像只有一个，因此不累加私有存储器图像(LocalMap)
		//后续需要偏置处理，因此结果输出到私有存储器中
		//卷积大小为Padding后的大小，即上下左右补零后的大小，即高宽各加2
		accconvolution(26, 50, 1, 0);
#ifdef HLS_DEBUG
		accconvolution_hps(26, 50, 1, 0);
		INFO_TRACE("accconvolution debug");
		HLS_TRACE(sram_hps + FPGA_LOCALMAP, acc_data_virtual_base + FPGA_LOCALMAP, 48 * 24);
#endif
		//偏置与激活
		//加载偏置数据到FPGA的Weight存储器中
		//基地址偏移为FPGA_Weight
		*(acc_data_virtual_base + FPGA_Weight) = seq == 0 ? para.chnlayer1bias[channel] : para.chrlayer1bias[channel];
#ifdef HLS_DEBUG
		*(sram_hps + FPGA_Weight) = seq == 0 ? para.chnlayer1bias[channel] : para.chrlayer1bias[channel];
#endif
		//配置加速器
		//由于卷积输出保存在私有存储器中，因此偏置激活的输入数据来自于私有存储器
		//后续需要池化处理，因此输出到私有存储器
		//偏置激活的图像大小为卷积后图像大小，与原始输入图像保持相同
		//执行
		accbiasrelu(24, 48, 1, 1);
#ifdef HLS_DEBUG
		accbiasrelu_hps(24, 48, 1, 1);
		INFO_TRACE("accbiasrelu debug");
		HLS_TRACE(sram_hps + FPGA_LOCALMAP, acc_data_virtual_base + FPGA_LOCALMAP, 48 * 24);
#endif
		//池化
		//配置加速器
		//由于偏置激活结果保存至私有存储器中，因此池化输入来自私有存储器
		//池化完成后第一层结束，后续无需任何操作，因此输出到FPGA Result存储器中
		//基地址偏移为FPGA_Result
		//池化图像大小与偏置激活后图像大小保持相同
		//执行
		accpool(24, 48, 1, 0);
#ifdef HLS_DEBUG
		accpool_hps(24, 48, 1, 0);
		INFO_TRACE("accpool debug");
		HLS_TRACE(sram_hps + FPGA_Result, acc_data_virtual_base + FPGA_Result, 24 * 12);
#endif
		//读取结果
		//输出图像大小为池化后大小，即高宽各减半(24*12)
		for (i = 0; i < 24; i++) {
			for (j = 0; j < 12; j++) {
				layer1poolout[channel][i][j] = *(acc_data_virtual_base
						+ FPGA_Result + i * 12 + j);
#ifdef HLS_DEBUG
				layer1poolout_hps[channel][i][j] = *(sram_hps
						+ FPGA_Result + i * 12 + j);
#endif
			}
		}
	}

	//----------------------------------------------
	//  第二卷积层
	//----------------------------------------------
	//与第一层卷积不同的是
	//第二层卷积有多个输入通道
	//因此每次独立卷积时都需要单独加载输入图像
	//L2CHAN个输出特征图
	INFO_TRACE("level 2 accconvolution");
	for (outchannel = 0; outchannel < L2CHAN; outchannel++) {
		//L1CHAN个输入特征图进行卷积并累加
		for (inchannel = 0; inchannel < L1CHAN; inchannel++) {
			//卷积
			//加载特征图
			//Padding，与第一层卷积的Padding类似
			for (i = 0; i < 26; i++) {
				for (j = 0; j < 14; j++) {
					if ((i == 0) || (i == 25)) {
						*(acc_data_virtual_base + i * 14 + j) = 0.0;
#ifdef HLS_DEBUG
						*(sram_hps + i * 14 + j) = 0.0;
#endif
					} else {
						if ((j == 0) || (j == 13)) {
							*(acc_data_virtual_base + i * 14 + j) = 0.0;
#ifdef HLS_DEBUG
							*(sram_hps + i * 14 + j) = 0.0;
#endif
						} else {
							*(acc_data_virtual_base + i * 14 + j) =
									layer1poolout[inchannel][i - 1][j - 1];
#ifdef HLS_DEBUG
							*(sram_hps + i * 14 + j) = layer1poolout_hps[inchannel][i - 1][j - 1];
#endif
						}
					}
				}
			}
			//加载权重
			for (i = 0; i < ConvSize; i++) {
				*(acc_data_virtual_base + FPGA_Weight + i) = seq == 0 ? para.chnlayer2weight[outchannel][inchannel][i] : para.chrlayer2weight[outchannel][inchannel][i];
#ifdef HLS_DEBUG
				*(sram_hps + FPGA_Weight + i) = seq == 0 ? para.chnlayer2weight[outchannel][inchannel][i] : para.chrlayer2weight[outchannel][inchannel][i];
#endif
			}
			//配置加速器
			//输出到私有存储器
			//每一个输出特征图，需要所有输入特征图分别卷积并累加结果
			//第一次计算不累加，其余累加
			//执行
			if (inchannel == 0){
				accconvolution(14, 26, 1, 0);
#ifdef HLS_DEBUG
				accconvolution_hps(14, 26, 1, 0);
				INFO_TRACE("accconvolution debug");
				HLS_TRACE(sram_hps + FPGA_LOCALMAP, acc_data_virtual_base + FPGA_LOCALMAP, 24 * 12);
#endif
			}
			else{
				accconvolution(14, 26, 1, 1);
#ifdef HLS_DEBUG
				accconvolution_hps(14, 26, 1, 1);
				INFO_TRACE("accconvolution debug");
				HLS_TRACE(sram_hps + FPGA_LOCALMAP, acc_data_virtual_base + FPGA_LOCALMAP, 24 * 12);
#endif
			}
		}

		//注意
		//此处inchannel循环已经结束
		//已经对所有输入特征图进行卷积并累加
		//配置加速器
		//来自私有存储器
		//输出到私有存储器
		*(acc_data_virtual_base + FPGA_Weight) = seq == 0 ? para.chnlayer2bias[outchannel] : para.chrlayer2bias[outchannel];
#ifdef HLS_DEBUG
		*(sram_hps + FPGA_Weight) = seq == 0 ? para.chnlayer2bias[outchannel] : para.chrlayer2bias[outchannel];
#endif
		//执行
		accbiasrelu(12, 24, 1, 1);
#ifdef HLS_DEBUG
		accbiasrelu_hps(12, 24, 1, 1);
		INFO_TRACE("accbiasrelu debug");
		HLS_TRACE(sram_hps + FPGA_LOCALMAP, acc_data_virtual_base + FPGA_LOCALMAP, 24 * 12);
#endif
		//池化
		//配置加速器
		//来自私有存储器
		//输出到Result
		//执行
		accpool(12, 24, 1, 0);
#ifdef HLS_DEBUG
		accpool_hps(12, 24, 1, 0);
		INFO_TRACE("accpool debug");
		HLS_TRACE(sram_hps + FPGA_Result, acc_data_virtual_base + FPGA_Result, 12 * 6);
#endif
		//取回池化结果
		for (i = 0; i < 12; i++) {
			for (j = 0; j < 6; j++) {
				layer2poolout[outchannel][i][j] = *(acc_data_virtual_base
						+ FPGA_Result + i * 6 + j);
#ifdef HLS_DEBUG
				layer2poolout_hps[outchannel][i][j] = *(sram_hps
						+ FPGA_Result + i * 6 + j);
#endif
			}
		}
	}

	//----------------------------------------------
	//  第三卷积层
	//----------------------------------------------
	//第三卷积层与第二卷积层类似
	INFO_TRACE("level 3 accconvolution");
	for (outchannel = 0; outchannel < L3CHAN; outchannel++) {
		for (inchannel = 0; inchannel < L2CHAN; inchannel++) {
			for (i = 0; i < 14; i++) {
				for (j = 0; j < 8; j++) {
					if ((i == 0) || (i == 13)) {
						*(acc_data_virtual_base + i * 8 + j) = 0.0;
#ifdef HLS_DEBUG
						*(sram_hps + i * 8 + j) = 0.0;
#endif
					} else {
						if ((j == 0) || (j == 7)) {
							*(acc_data_virtual_base + i * 8 + j) = 0.0;
#ifdef HLS_DEBUG
							*(sram_hps + i * 8 + j) = 0.0;
#endif
						} else {
							*(acc_data_virtual_base + i * 8 + j) =
									layer2poolout[inchannel][i - 1][j - 1];
#ifdef HLS_DEBUG
							*(sram_hps + i * 8 + j) =
									layer2poolout_hps[inchannel][i - 1][j - 1];
#endif
						}
					}
				}
			}
			for (i = 0; i < ConvSize; i++) {
				*(acc_data_virtual_base + FPGA_Weight + i) = seq == 0 ? para.chnlayer3weight[outchannel][inchannel][i] : para.chrlayer3weight[outchannel][inchannel][i];
#ifdef HLS_DEBUG
				*(sram_hps + FPGA_Weight + i) = seq == 0 ? para.chnlayer3weight[outchannel][inchannel][i] : para.chrlayer3weight[outchannel][inchannel][i];
#endif
			}
			if (inchannel == 0){
				accconvolution(8, 14, 1, 0);
#ifdef HLS_DEBUG
				accconvolution_hps(8, 14, 1, 0);
				INFO_TRACE("accconvolution debug");
				HLS_TRACE(sram_hps + FPGA_LOCALMAP, acc_data_virtual_base + FPGA_LOCALMAP, 12 * 6);
#endif
			}
			else{
				accconvolution(8, 14, 1, 1);
#ifdef HLS_DEBUG
				accconvolution_hps(8, 14, 1, 1);
				INFO_TRACE("accconvolution debug");
				HLS_TRACE(sram_hps + FPGA_LOCALMAP, acc_data_virtual_base + FPGA_LOCALMAP, 12 * 6);
#endif
			}
		}
		*(acc_data_virtual_base + FPGA_Weight) = seq == 0 ? para.chnlayer3bias[outchannel] : para.chrlayer3bias[outchannel];
#ifdef HLS_DEBUG
		*(sram_hps + FPGA_Weight) = seq == 0 ? para.chnlayer3bias[outchannel] : para.chrlayer3bias[outchannel];
#endif
		accbiasrelu(6, 12, 1, 1);
#ifdef HLS_DEBUG
		accbiasrelu_hps(6, 12, 1, 1);
		INFO_TRACE("accbiasrelu debug");
		HLS_TRACE(sram_hps + FPGA_LOCALMAP, acc_data_virtual_base + FPGA_LOCALMAP, 12 * 6);
#endif
		accpool(6, 12, 1, 0);
#ifdef HLS_DEBUG
		accpool_hps(6, 12, 1, 0);
		INFO_TRACE("accpool debug");
		HLS_TRACE(sram_hps + FPGA_Result, acc_data_virtual_base + FPGA_Result, 6 * 3);
#endif
		for (i = 0; i < 6; i++) {
			for (j = 0; j < 3; j++) {
				layer3poolout[outchannel][i][j] = *(acc_data_virtual_base
						+ FPGA_Result + i * 3 + j);
#ifdef HLS_DEBUG
				layer3poolout_hps[outchannel][i][j] = *(sram_hps
						+ FPGA_Result + i * 3 + j);
#endif
			}
		}
	}

	//----------------------------------------------
	//  第一全连接层
	//----------------------------------------------
	//全连接运算被拆分成16次乘累加的组合
	//总共NER个输出神经元
	INFO_TRACE("level 1 accfullconnect");
	for (ner = 0; ner < NER; ner++) {
		//初始化神经元
		neural_result[ner] = 0;
#ifdef HLS_DEBUG
		neural_result_hps[ner] = 0;
#endif
		//遍历每个输入特征图
		for (channel = 0; channel < L3CHAN; channel++) {
			//行列计数器
			//使用行列计数器来遍历特征图的每个点
			int rowcnt = 0;
			int colcnt = 0;
			for (i = 0; i < 2; i++) {
				for (j = 0; j < 16; j++) {
					//将数据与权重写入FPGA中的存储器
					//判断行列计数器是否大于图像大小，即第二次乘累加的补零
					if ((rowcnt * 3 + colcnt) > 17) {
						*(acc_data_virtual_base + j) = 0.0;
						*(acc_data_virtual_base + FPGA_Weight + j) = 0.0;
#ifdef HLS_DEBUG
						*(sram_hps + j) = 0.0;
						*(sram_hps + FPGA_Weight + j) = 0.0;
#endif
					} else {
						*(acc_data_virtual_base + j) =
								layer3poolout[channel][rowcnt][colcnt];
						*(acc_data_virtual_base + FPGA_Weight + j) = seq == 0 ? para.chnfull1weight[ner][channel][rowcnt][colcnt] : para.chrfull1weight[ner][channel][rowcnt][colcnt];
#ifdef HLS_DEBUG
						*(sram_hps + j) =
								layer3poolout_hps[channel][rowcnt][colcnt];
						*(sram_hps + FPGA_Weight + j) = seq == 0 ? para.chnfull1weight[ner][channel][rowcnt][colcnt] : para.chrfull1weight[ner][channel][rowcnt][colcnt];
#endif
					}
					if (colcnt == 2) {
						colcnt = 0;
						rowcnt++;
					} else {
						colcnt++;
					}
				}
				//配置加速器
				//执行
				accfullconnect();
#ifdef HLS_DEBUG
				accfullconnect_hps();
				INFO_TRACE("accfullconnect debug");
				HLS_TRACE(sram_hps + FPGA_Result, acc_data_virtual_base + FPGA_Result, 1);
#endif
				//读取结果
				neural_result[ner] += *(acc_data_virtual_base + FPGA_Result);
#ifdef HLS_DEBUG
				neural_result_hps[ner] += *(sram_hps + FPGA_Result);
#endif
			}
		}
		//加偏置
		//此处channel循环已经结束
		//这里的加偏执由HPS完成
		neural_result[ner] += seq == 0 ? para.chnnerbias[ner] : para.chrnerbias[ner];
		//激活，同样由HPS完成
		neural_result[ner] =
				(neural_result[ner] > 0) ? neural_result[ner] : 0.0;
#ifdef HLS_DEBUG
		neural_result_hps[ner] += seq == 0 ? para.chnnerbias[ner] : para.chrnerbias[ner];
		neural_result_hps[ner] =
				(neural_result_hps[ner] > 0) ? neural_result_hps[ner] : 0.0;
#endif
	}

	//----------------------------------------------
	//  第二全连接层
	//----------------------------------------------
	//  第二次全连接同样将运算拆分为16次乘累加的组合
	INFO_TRACE("level 2 accfullconnect");
	int outner, group, inner;
	//输出结果个数为ChrCls
	for (outner = 0; outner < ChrCls; outner++) {
		//初始化输出结果数组
		probability[outner] = 0;
#ifdef HLS_DEBUG
		probability_hps[outner] = 0;
#endif
		//由于第一次全连接输出了64个神经原，因此拆分成4x16次运算
		for (group = 0; group < 4; group++) {
			//加载16次乘累加的数据与权重
			for (inner = 0; inner < 16; inner++) {
				*(acc_data_virtual_base + inner) = neural_result[inner
						+ group * 16];
				*(acc_data_virtual_base + inner + FPGA_Weight) = seq == 0 ? para.chnfull2weight[outner][inner + group * 16] : para.chrfull2weight[outner][inner + group * 16];
				//执行
				accfullconnect();
#ifdef HLS_DEBUG
				*(sram_hps + inner) = neural_result_hps[inner
						+ group * 16];
				*(sram_hps + inner + FPGA_Weight) = seq == 0 ? para.chnfull2weight[outner][inner + group * 16] : para.chrfull2weight[outner][inner + group * 16];
				//执行
				accfullconnect_hps();
				INFO_TRACE("accfullconnect debug");
				HLS_TRACE(sram_hps + FPGA_Result, acc_data_virtual_base + FPGA_Result, 1);
#endif
			}
			//读取结果并由HPS完成4次结果的累加
			probability[outner] += *(acc_data_virtual_base + FPGA_Result);
#ifdef HLS_DEBUG
			probability_hps[outner] += *(sram_hps + FPGA_Result);
#endif
		}
		//加偏置
		probability[outner] += seq == 0 ? para.chnprobias[outner] : para.chrprobias[outner];
#ifdef HLS_DEBUG
		probability_hps[outner] += seq == 0 ? para.chnprobias[outner] : para.chrprobias[outner];
#endif
	}

	//----------------------------------------------
	//  得到最终结果
	//----------------------------------------------
	//找到结果数组中最大的值，其对应的下标就是识别结果
	int result = 0;
	int max = probability[0];
#ifdef HLS_DEBUG
	int result_hps = 0;
	int max_hps = probability_hps[0];
#endif

	if(seq == 0){
		for (i = 0; i < ChnCls; i++) {
			if (probability[i] > max) {
				max = probability[i];
				result = i;
			}
		}
	}
	else{
		for (i = 0; i < ChrCls; i++) {
			if (probability[i] > max) {
				max = probability[i];
				result = i;
			}
		}
	}
	/**
	 * @Date: 2021-04-17 20:09:40
	 * @Author: Ciao
	 * new add
	 */
	if(onenum == seq)
		strcpy(chrret, "1");
	else
		chrfind(seq, result);
	VALUEs_TRACE("chrinfer", chrret);
	strcat(license, chrret);
#ifdef HLS_DEBUG
	if(seq == 0){
		for (i = 0; i < ChnCls; i++) {
			if (probability_hps[i] > max_hps) {
				max_hps = probability_hps[i];
				result_hps = i;
			}
		}
	}
	else{
		for (i = 0; i < ChrCls; i++) {
			if (probability_hps[i] > max_hps) {
				max_hps = probability_hps[i];
				result_hps = i;
			}
		}
	}
	chrfind(seq, result_hps);
	VALUEs_TRACE("hps chrinfer", chrret);
	strcat(license_hps, chrret);
#endif

	return (result);
}

#undef _INFER_GLOBLE
