#include "stdio.h"
#include"Best_Code.h"
#include <string.h>
void Best_Code(unsigned char Light_Code[CODE_LEN],unsigned char decode[CODE_NUM_MAX][CODE_LEN],int code_num)
{
	int bestcode_loc = 0;
	int num = 0, same = 1, mostnum = 1;
	int i,j,k;
	for(i = 0;i<code_num;i++)
	{
		num = 0;
		for(j=0;j<code_num;j++)
		{
			same = 1;
			for(k=0;k<7;k++)
			{
				if(decode[i][k]!=decode[j][k])
				{
					same = 0; break;
				}
			}
			if(same==1) num++;
		}
		if(mostnum<num)
		{
			mostnum = num;
			bestcode_loc = i;
		}
	}
	if(code_num==0){
		memset(Light_Code,0,CODE_LEN);
	}
	else{
		memcpy(Light_Code,decode[bestcode_loc],CODE_LEN);
	}
	//return decode[bestcode_loc];
}