/**
  ******************************************************************************
  * File Name          : DL_bpbet.h
  * Description        : use bpnet to train and sim 
  * Data               : 2018/4/23
	* Hardward           : 
	* Important functions {  BpNet_Init()    
                           BPtrain(a,b,c)  "a" defined a[训练/预测数据数量][输入维度]
	                         BPsim(a,b,c)    "b" defined b[训练/预测数据数量][输出维度] 
	                                         "c" 训练/预测数据数量
                	      }
	*****************************************************************************
	*/
/*
ceil()是#include <math.h>中的函数,虽然程序中已经包含了该头文件,但是编译的时候还是说这个函数没有定义(也就是"对ceil未定义的引用)

需要重新编译,并在最后加上"-lm
 gcc -o DL_bpnet DL_bpnet.c -lm
*/
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include "HW_adc.h"                //用来采集随机数
//#include "arm_math.h"          //dsp

//extern int rand_seed;    //产生的随机值

#define InLayerNodesNum    3//输入层节点数
#define MidLayerNodesNum   8//隐层节点数
#define OutLayerNodesNum   3


void normalize(double **t, int size);
	
void unnormalize(double **t, int size);

void BpNet_Init(void);
	

int BPtrain(double(*p)[InLayerNodesNum],  double(*t)[OutLayerNodesNum], int size);

int BPsim(double(*p)[InLayerNodesNum], double(*t)[OutLayerNodesNum], int size);
	
