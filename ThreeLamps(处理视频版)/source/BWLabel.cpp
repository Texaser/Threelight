#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>



//////////////////////////////////////////////////////////
//��ջ����
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




// ������ջ����СΪSTACK_INIT_SIZE
Stack* InitStack( void )
{
	Stack *pStack = (Stack*)calloc( 1, sizeof(Stack) );
	pStack->base = (int**)calloc( STACK_INIT_SIZE+1, sizeof(int *) );
	pStack->top = pStack->base;
	pStack->stacksize = STACK_INIT_SIZE;

	return pStack;
}




// �ж�ջ�Ƿ�Ϊ��
int IsStackEmpty( Stack *pStack )
{
	// ջ�յĵ������� pStack->base != NULL���� pStack->top == pStack->base
	if( pStack->top == pStack->base )
		return TRUE ;
	else
		return FALSE ;
} 




// ��Ԫ��pnElementѹ��ջ��
void Push( Stack *pStack , int *pnElement )
{
	// ��ջ���������ӿռ�
	// ����BUG
	if( pStack->top - pStack->base >= pStack->stacksize )
	{
		pStack->base = (int **)realloc( pStack->base, (pStack->stacksize+STACK_INCRE)*sizeof(int *) );
		pStack->top = pStack->base + pStack->stacksize;
		pStack->stacksize += STACK_INCRE;
	}

	++ pStack->top;
	*(pStack->top) = pnElement;
}




// ɾ��ջ��Ԫ�أ�����pnElement������ֵ
void Pop( Stack *pStack, int *pnElement ) 
{
	if( pStack->top == pStack->base && *(pStack->top)==NULL  )
		printf("ջΪ�գ�\n");
	else
	{
		memcpy( pnElement, *(pStack->top), 4*sizeof(int) );
		(pStack->top)--;
	}
}



// ���ٶ�ջ
void DestroyStack( Stack *pStack )
{
	free( pStack->base );
	pStack->top = pStack->base = NULL ;
	pStack->stacksize = 0;

	free( pStack ), pStack=NULL;
}

/************************************************************************/
/* 
������BWLabel
���ܣ�ʵ�ֶ�ֵͼ�����ͨ������
������
pnBW---ָ���ֵͼ�����ݣ�Ҫ������ֵΪ0��1 (���ڸ�Ϊ0/255)
pnBWLabel---ָ���Ǻ��ͼ��
nImageWidth��nImageHeight---ͼ��߿�
nMode---���������ģʽ��8Ϊ����ͨ��4Ϊ����ͨ

���أ�ͼ����������Ŀ
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
		printf("�����ƶ�nMode��ֵ��������8����4��");
		exit(-1);
	}

	//�ӱߣ�����
	pnBWPadded = (int*)calloc( nWidthPadded*nHeightPadded, sizeof(int) );
	for ( i=0; i<nImageHeight; i++ )
		memcpy( pnBWPadded+(i+1)*nWidthPadded+1, pnBW+i*nImageWidth, nImageWidth*sizeof(int) );

	//ͳ��ÿһ���߶ε���Ŀ������pnLineNumber
	pnSegNumPerLine = (int*)calloc( nHeightPadded, sizeof(int) );
	for ( i=0; i<nImageHeight; i++ )
		for ( j=0; j<nWidthPadded-1; j++ )
		{
			if ( pnBWPadded[(i+1)*nWidthPadded+j]==0 && pnBWPadded[(i+1)*nWidthPadded+j+1]==1 )
				pnSegNumPerLine[i+1]++;
		}

		//Ϊÿһ�е��߶ζ˵�洢����ռ�
		pnSegments = (int**)calloc( nHeightPadded, sizeof(int* ) );
		for ( i=0; i<nHeightPadded; i++ )
			pnSegments[i] = (int*)calloc( pnSegNumPerLine[i]*4, sizeof(int) );
		//ɨ����
		/*pnSegment�����е����ݽṹ*/
		/*---------------------------
		|�к�|��˵�|�Ҷ˵�|��־λ|
		---------------------------
		|�к�|��˵�|�Ҷ˵�|��־λ|
		---------------------------
		|�к�|��˵�|�Ҷ˵�|��־λ|
		---------------------------
		|�к�|��˵�|�Ҷ˵�|��־λ|
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
					pnSegments[i][n*4  ] = i;  //��¼�к�
					pnSegments[i][n*4+1] = j;  //��¼��˵�
					pnSegments[i][n*4+2] = k-1;//��¼�Ҷ˵�
					n++;
					j = k;
				}
			}
		}

		pStack = InitStack();//��ʼ����ջ
		N = 1;//����ı��
		for ( i=1; i<nHeightPadded-1; i++ )
		{
			for ( j=0; j<pnSegNumPerLine[i]; j++ )
			{
				//�߶�δ�������δ��ɨ�裬���µ�һ��
				if ( pnSegments[i][j*4+3]==0 )
				{
					pnSegmentTemp = &pnSegments[i][j*4];
					//*(pStack->top) = pnSegmentTemp;
					pnSegmentTemp[3] = -1;//��־λ��-1��ʾ��ջ
					Push( pStack, pnSegmentTemp );//��ջ
Loop:	
					pnSegmentTemp = *(pStack->top);
					X = pnSegmentTemp[0];//�к�
					L = pnSegmentTemp[1];//��˵�
					R = pnSegmentTemp[2];//�Ҷ˵�

					//�����ǰ���ͨ��������ͨ
					LL = (nMode==CONNECTIVITY8)?(L-1):L;
					RR = (nMode==CONNECTIVITY8)?(R+1):R;

					nFlag = 0;
					//ɨ����һ�п��Ƿ����δ��ǵ��ڽ��߶Σ����ڣ�nFlag=1�����ڽӶ�ѹ���ջ
					for ( m=0; m<pnSegNumPerLine[X-1]; m++ )
					{
						if ( pnSegments[X-1][m*4+1]<=RR && pnSegments[X-1][m*4+2]>=LL && pnSegments[X-1][m*4+3]==0 )
						{
							pnSegments[X-1][m*4+3] = -1;
							Push( pStack, &pnSegments[X-1][m*4] );
							nFlag = 1;
						}
					}
					//ɨ����һ�п��Ƿ�����ڽ��߶Σ����ڣ�nFlag=1�����ڽӶ�ѹ���ջ
					for ( n=0; n<pnSegNumPerLine[X+1]; n++ )
					{
						if ( pnSegments[X+1][n*4+1]<=RR && pnSegments[X+1][n*4+2]>=LL && pnSegments[X+1][n*4+3]==0 )
						{
							pnSegments[X+1][n*4+3] = -1;
							Push( pStack, &pnSegments[X+1][n*4] );
							nFlag = 1;
						}
					}

					//�����ö��ڽӶξ��ѱ��
					//���߲������ڽӶ�
					if ( nFlag==0 )
					{
						if ( IsStackEmpty( pStack ) )//ջΪ�գ����������һ����ͨ����
						{
							N++;
							continue;
						}		
						else
						{
							(*(pStack->top))[3] = N;//ջΪ���գ���ǵ�ǰ��ͨ����
							Pop( pStack, pnSegmentTemp );//����Ӷ�ջ�е���

							//ջΪ�գ�������ǰ��ͨ���������
							//��ʼ��һ����ͨ����ı��
							if ( IsStackEmpty( pStack ) )
							{
								N++;
								continue;
							}
							else
								goto Loop;//ջ��Ϊ�գ���������ջ��Ԫ��
						}
					}
					else
						goto Loop;//��������ջ��Ԫ��
				}
			}
		}

		//����pnSegments�и������λ����Ƕ�ֵͼ��
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

