#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>



//////////////////////////////////////////////////////////
//堆栈操作
#define TRUE 1
#define FALSE 0
#define STACK_INIT_SIZE 1000
#define STACK_INCRE 200

#define CONNECTIVITY8 8
#define CONNECTIVITY4 4



typedef struct tagStack
{
	int **base;
	int **top;
	int stacksize;
}Stack;




// 建立空栈，大小为STACK_INIT_SIZE
Stack* InitStack( void )
{
	Stack *pStack = (Stack*)calloc( 1, sizeof(Stack) );
	pStack->base = (int**)calloc( STACK_INIT_SIZE+1, sizeof(int *) );
	pStack->top = pStack->base;
	pStack->stacksize = STACK_INIT_SIZE;

	return pStack;
}




// 判断栈是否为空
int IsStackEmpty( Stack *pStack )
{
	// 栈空的的条件是 pStack->base != NULL，且 pStack->top == pStack->base
	if( pStack->top == pStack->base )
		return TRUE ;
	else
		return FALSE ;
} 




// 把元素pnElement压入栈中
void Push( Stack *pStack , int *pnElement )
{
	// 若栈满，则增加空间
	// 存在BUG
	if( pStack->top - pStack->base >= pStack->stacksize )
	{
		pStack->base = (int **)realloc( pStack->base, (pStack->stacksize+STACK_INCRE)*sizeof(int *) );
		pStack->top = pStack->base + pStack->stacksize;
		pStack->stacksize += STACK_INCRE;
	}

	++ pStack->top;
	*(pStack->top) = pnElement;
}




// 删除栈顶元素，并用pnElement返回其值
void Pop( Stack *pStack, int *pnElement ) 
{
	if( pStack->top == pStack->base && *(pStack->top)==NULL  )
		printf("栈为空！\n");
	else
	{
		memcpy( pnElement, *(pStack->top), 4*sizeof(int) );
		(pStack->top)--;
	}
}



// 销毁堆栈
void DestroyStack( Stack *pStack )
{
	free( pStack->base );
	pStack->top = pStack->base = NULL ;
	pStack->stacksize = 0;

	free( pStack ), pStack=NULL;
}

/************************************************************************/
/* 
函数：BWLabel
功能：实现二值图像的连通区域标记
参数：
pnBW---指向二值图像数据，要求像素值为0、1 (现在改为0/255)
pnBWLabel---指向标记后的图像
nImageWidth、nImageHeight---图像高宽
nMode---邻域的连接模式：8为八连通，4为四连通

返回：图像中区域数目
*/
/************************************************************************/
int BWLabel( int *pnBW, int *pnBWLabel, int nImageWidth, int nImageHeight, int nMode )
{
	int i=0, j=0, k=0, m=0, n=0, L=0, R=0, LL=0, RR=0, N=0, X=0, nFlag=0;
	int *pnBWPadded=NULL, nWidthPadded=nImageWidth+2, nHeightPadded=nImageHeight+2;
	int *pnSegNumPerLine=NULL, **pnSegments=NULL, *pnSegmentTemp=NULL;

	Stack *pStack = NULL;

	if ( nMode!=CONNECTIVITY4 && nMode!=CONNECTIVITY8 )
	{
		printf("必须制定nMode的值，必须是8或者4！");
		exit(-1);
	}

	//加边，补零
	pnBWPadded = (int*)calloc( nWidthPadded*nHeightPadded, sizeof(int) );
	for ( i=0; i<nImageHeight; i++ )
		memcpy( pnBWPadded+(i+1)*nWidthPadded+1, pnBW+i*nImageWidth, nImageWidth*sizeof(int) );

	//统计每一行线段的数目，存入pnLineNumber
	pnSegNumPerLine = (int*)calloc( nHeightPadded, sizeof(int) );
	for ( i=0; i<nImageHeight; i++ )
		for ( j=0; j<nWidthPadded-1; j++ )
		{
			if ( pnBWPadded[(i+1)*nWidthPadded+j]==0 && pnBWPadded[(i+1)*nWidthPadded+j+1]==1 )
				pnSegNumPerLine[i+1]++;
		}

		//为每一行的线段端点存储分配空间
		pnSegments = (int**)calloc( nHeightPadded, sizeof(int* ) );
		for ( i=0; i<nHeightPadded; i++ )
			pnSegments[i] = (int*)calloc( pnSegNumPerLine[i]*4, sizeof(int) );
		//扫描标记
		/*pnSegment矩阵中的数据结构*/
		/*---------------------------
		|行号|左端点|右端点|标志位|
		---------------------------
		|行号|左端点|右端点|标志位|
		---------------------------
		|行号|左端点|右端点|标志位|
		---------------------------
		|行号|左端点|右端点|标志位|
		---------------------------*/
		for ( i=1; i<nHeightPadded-1; i++ )
		{
			n = 0;
			for ( j=0; j<nWidthPadded-1; j++ )
			{
				if ( pnBWPadded[i*nWidthPadded+j]==1 )
				{
					for ( k=j+1; k<nWidthPadded; k++ )
					{
						if ( pnBWPadded[i*nWidthPadded+k]==0 )
							break;
					}
					pnSegments[i][n*4  ] = i;  //记录行号
					pnSegments[i][n*4+1] = j;  //记录左端点
					pnSegments[i][n*4+2] = k-1;//记录右端点
					n++;
					j = k;
				}
			}
		}

		pStack = InitStack();//初始化堆栈
		N = 1;//区域的编号
		for ( i=1; i<nHeightPadded-1; i++ )
		{
			for ( j=0; j<pnSegNumPerLine[i]; j++ )
			{
				//线段未被标记且未被扫描，即新的一段
				if ( pnSegments[i][j*4+3]==0 )
				{
					pnSegmentTemp = &pnSegments[i][j*4];
					//*(pStack->top) = pnSegmentTemp;
					pnSegmentTemp[3] = -1;//标志位置-1表示入栈
					Push( pStack, pnSegmentTemp );//入栈
Loop:	
					pnSegmentTemp = *(pStack->top);
					X = pnSegmentTemp[0];//行号
					L = pnSegmentTemp[1];//左端点
					R = pnSegmentTemp[2];//右端点

					//区分是八连通还是四连通
					LL = (nMode==CONNECTIVITY8)?(L-1):L;
					RR = (nMode==CONNECTIVITY8)?(R+1):R;

					nFlag = 0;
					//扫描上一行看是否存在未标记的邻接线段，存在，nFlag=1，将邻接段压入堆栈
					for ( m=0; m<pnSegNumPerLine[X-1]; m++ )
					{
						if ( pnSegments[X-1][m*4+1]<=RR && pnSegments[X-1][m*4+2]>=LL && pnSegments[X-1][m*4+3]==0 )
						{
							pnSegments[X-1][m*4+3] = -1;
							Push( pStack, &pnSegments[X-1][m*4] );
							nFlag = 1;
						}
					}
					//扫描下一行看是否存在邻接线段，存在，nFlag=1，将邻接段压入堆栈
					for ( n=0; n<pnSegNumPerLine[X+1]; n++ )
					{
						if ( pnSegments[X+1][n*4+1]<=RR && pnSegments[X+1][n*4+2]>=LL && pnSegments[X+1][n*4+3]==0 )
						{
							pnSegments[X+1][n*4+3] = -1;
							Push( pStack, &pnSegments[X+1][n*4] );
							nFlag = 1;
						}
					}

					//表明该段邻接段均已标记
					//或者不存在邻接段
					if ( nFlag==0 )
					{
						if ( IsStackEmpty( pStack ) )//栈为空，继续标记下一个连通区域
						{
							N++;
							continue;
						}		
						else
						{
							(*(pStack->top))[3] = N;//栈为不空，标记当前连通区域
							Pop( pStack, pnSegmentTemp );//将其从堆栈中弹出

							//栈为空，表明当前连通区域标记完毕
							//开始下一个连通区域的标记
							if ( IsStackEmpty( pStack ) )
							{
								N++;
								continue;
							}
							else
								goto Loop;//栈不为空，继续处理栈顶元素
						}
					}
					else
						goto Loop;//继续处理栈顶元素
				}
			}
		}

		//依据pnSegments中各个标记位，标记二值图像
		for ( i=0; i<nImageHeight; i++ )
			for ( j=0; j<pnSegNumPerLine[i+1]; j++ )
				for ( k=pnSegments[i+1][j*4+1]; k<=pnSegments[i+1][j*4+2]; k++ )
					pnBWLabel[i*nImageWidth+k-1] = pnSegments[i+1][j*4+3];

		free( pnBWPadded ), pnBWPadded = NULL;
		free( pnSegNumPerLine ), pnSegNumPerLine = NULL;
		for ( i=0; i<nHeightPadded; i++ )
		{
			if ( pnSegments[i]!=NULL )
			{
				free(pnSegments[i]); 
				pnSegments[i]=NULL;
			}
		}
		free(pnSegments), pnSegments=NULL;
		DestroyStack( pStack );

		return N-1;
}

