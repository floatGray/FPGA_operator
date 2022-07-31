#define _HLS_HPS_GLOBLE

#include "include.h"

void accconvolution_hps(int width, int height, int outdir, int accmulate){
#ifdef HLS_DEBUG
    float*  maps = sram_hps;
    float*  Weight = sram_hps + FPGA_Weight;
    float*  LocalMap = sram_hps + FPGA_LOCALMAP;
    float*  Result = sram_hps + FPGA_Result;
    float   Window[3][3];
    float   temp;

    int _height = height - 2;
    int _width = width - 2;
    int index = 0;

    int h, w;

    for(h = 0;h < _height; h++) {
        for(w = 0;w < _width; w++) {

            temp    =   maps[ (h    ) * width + w     ]     * Weight[0]    +
                        maps[ (h    ) * width + w + 1 ]     * Weight[1]    +
                        maps[ (h    ) * width + w + 2 ]     * Weight[2]    +
                        maps[ (h + 1) * width + w     ]     * Weight[3]    +
                        maps[ (h + 1) * width + w + 1 ]     * Weight[4]    +
                        maps[ (h + 1) * width + w + 2 ]     * Weight[5]    +
                        maps[ (h + 2) * width + w     ]     * Weight[6]    +
                        maps[ (h + 2) * width + w + 1 ]     * Weight[7]    +
                        maps[ (h + 2) * width + w + 2 ]     * Weight[8]    ;

            index = h * (width - 2) + w;
            if(accmulate)
                temp    +=  LocalMap[index];

            if(outdir)
                LocalMap[index] =   temp;
            else
                Result[index] =   temp;
        }
    }
#endif
}

void accbiasrelu_hps(int width, int height, int indir, int outdir){
#ifdef HLS_DEBUG
    float*  maps = sram_hps;
    float*  Weight = sram_hps + FPGA_Weight;
    float*  LocalMap = sram_hps + FPGA_LOCALMAP;
    float*  Result = sram_hps + FPGA_Result;
    int Exit    =   width * height;
    float temp;
    float *temp1;
    float *inputbuffer;
    if(indir)
    	inputbuffer =   LocalMap;
    else 
    	inputbuffer =   maps;

    if(outdir)
    	temp1 = LocalMap;
    else
	    temp1 = Result;

    int i;

    for(i = 0;i < Exit;i++) {
        temp            =   inputbuffer[i] + Weight[0];
        temp1[i]          =   (temp > 0)  ?   temp   :    0;
    }
#endif
}

void accpool_hps(int width, int height, int indir, int outdir){
#ifdef HLS_DEBUG
    float*  maps = sram_hps;
    float*  Weight = sram_hps + FPGA_Weight;
    float*  LocalMap = sram_hps + FPGA_LOCALMAP;
    float*  Result = sram_hps + FPGA_Result;
    float PoolWindow[2][2];
    float temp0;
    float temp1;
    float temp2;
    int _height = height >> 1;
    int _width = width >> 1;
    int index = 0;
    int _index = 0;
    int _index_1 = 0;

    float *input;
    float *output;
    if(indir)
        input = LocalMap;
    else
        input = maps;

    if(outdir)
        output = LocalMap;
    else
        output = Result;

    int h, w;

    for(h = 0;h < _height;h++) {
        for(w = 0;w < _width;w++) {
            _index = h * 2 * width + w * 2;
            _index_1 = (h * 2 + 1) * width + w * 2;
            PoolWindow[0][0] = input[ _index ];
            PoolWindow[0][1] = input[ _index + 1];
            PoolWindow[1][0] = input[ _index_1 ];
            PoolWindow[1][1] = input[ _index_1 + 1];

            temp0    =   (PoolWindow[0][0] > PoolWindow[0][1]) ? PoolWindow[0][0] : PoolWindow[0][1];
            temp1    =   (PoolWindow[1][0] > PoolWindow[1][1]) ? PoolWindow[1][0] : PoolWindow[1][1];
            temp2    =   (temp0 > temp1) ? temp0 : temp1;
            output[h*_width + w] = temp2;
        }
    }
#endif
}

void accfullconnect_hps(void){
#ifdef HLS_DEBUG
    float*  maps = sram_hps;
    float*  Weight = sram_hps + FPGA_Weight;
    float*  LocalMap = sram_hps + FPGA_LOCALMAP;
    float*  Result = sram_hps + FPGA_Result;
    float temp[16];
    float tmp = 0.0;
    float *result;
    result = Result;

    int i;

    for(i = 0;i < 16;i++)
    {
        temp[i] = maps[i] * Weight[i];
    }

    for(i=0; i< 16;i++)
    {
        tmp += temp[i];
    }

    result[0] = tmp;
#endif
}

#undef _HLS_HPS_GLOBLE
