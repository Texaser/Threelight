#pragma once

#define framenum   17 //¶ÁÈ¡Í¼Æ¬Êý
#define CODE_NUM_ONEPIC_MAX  6
#define CODE_NUM_MAX  CODE_NUM_ONEPIC_MAX*framenum
#define CODE_LEN  7
int EVA_dec(int** gray,int h,int w,unsigned char code[CODE_NUM_ONEPIC_MAX][CODE_LEN]);