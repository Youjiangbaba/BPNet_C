/**
  ******************************************************************************
  * File Name          : DL_bpbet.h
  * Description        : use bpnet to train and sim 
  * Data               : 2018/4/23
	* Hardward           : 
	* Important functions {  BpNet_Init()    
                           BPtrain(a,b,c)  "a" defined a[ѵ��/Ԥ����������][����ά��]
	                         BPsim(a,b,c)    "b" defined b[ѵ��/Ԥ����������][���ά��] 
	                                         "c" ѵ��/Ԥ����������
                	      }
	*****************************************************************************
	*/
/*
ceil()��#include <math.h>�еĺ���,��Ȼ�������Ѿ������˸�ͷ�ļ�,���Ǳ����ʱ����˵�������û�ж���(Ҳ����"��ceilδ���������)

��Ҫ���±���,����������"-lm
 gcc -o DL_bpnet DL_bpnet.c -lm
*/
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include "HW_adc.h"                //�����ɼ������
//#include "arm_math.h"          //dsp

//extern int rand_seed;    //���������ֵ

#define InLayerNodesNum    3//�����ڵ���
#define MidLayerNodesNum   8//����ڵ���
#define OutLayerNodesNum   3


void normalize(double **t, int size);
	
void unnormalize(double **t, int size);

void BpNet_Init(void);
	

int BPtrain(double(*p)[InLayerNodesNum],  double(*t)[OutLayerNodesNum], int size);

int BPsim(double(*p)[InLayerNodesNum], double(*t)[OutLayerNodesNum], int size);
	
