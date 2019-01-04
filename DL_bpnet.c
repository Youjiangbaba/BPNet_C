/**
  ******************************************************************************
  * File Name          : DL_bpbet.c
  * Description        : use bpnet to train and sim 
  * Data               : 2018/4/23
	* Hardward           : 
	* Important functions {  BpNet_Init()    
                           BPtrain(a,b,c)  "a" defined a[训练/预测数据数量][输入维度]
	                         BPsim(a,b,c)    "b" defined b[训练/预测数据数量][输出维度] 
	                                         "c" 训练/预测数据数量
                	      }
 *****************************************************************************
   *Modified 	              : 2019/1/4 by Jiang
   *Changes                   : add function : save and load net
				add testdata to test
   *Requires attention        : #define TRAINNING  1 : function train and save new net
				#define TRAINNING  0 : load existed net to sim
   				DL_bpbet.h to change LayerNodesNum 
				bp_net.h  to see the bp-nn-net
 *****************************************************************************
*/



#include "DL_bpnet.h"
#include "string.h"
#include "sample.h"
#include "testdata.h"
#define TRAINNING  1



double _m_w_rate ;//输入层-隐含层权值学习率
double _o_w_rate ;//隐含层-网络层权学习率
double _m_t_rate ;//输入层-隐含层阈值学习率
double _o_t_rate ;//隐含层-网络层阈值学习率
int _it_nums ;//迭代次数
double _err ;//误差限

double _m_threshold[MidLayerNodesNum];//输入层-隐含层阈值
double _o_threshold[OutLayerNodesNum];//隐含层-网络层阈值
double _m_weight[InLayerNodesNum][MidLayerNodesNum];//输入层-隐含层权值
double _o_weight[MidLayerNodesNum][OutLayerNodesNum];//隐含层-网络层权值
double _mint, _maxt;
	
const int N = 1000;
const int n = 200;


#if TRAINNING == 0
#include "bpnet.h"

void load_BPnet(void)
{
	int Innum = InLayerNodesNum;
	int Midnum = MidLayerNodesNum;
	int Outnum = OutLayerNodesNum;
	_m_w_rate = _m_w_rate_save;
	_o_w_rate = _o_w_rate_save;
	_m_t_rate = _m_t_rate_save;
	_o_t_rate = _o_t_rate_save;
	_it_nums = _it_nums;
	_err = _err;
	_mint = _mint_save;
	_maxt = _maxt_save;
	for(int i = 0;i < Midnum;i++)
		_m_threshold[i] = _m_threshold_save[i];
	for(int i = 0;i < Outnum;i++)
		_o_threshold[i] = _o_threshold_save[i];
	for(int i = 0;i < Innum;i++)
		for(int j=0;j< Midnum;j++)
			 _m_weight[i][j] =  _m_weight_save[i][j];
	for(int i = 0;i < Midnum;i++)
		for(int j=0;j< Outnum;j++)
			_o_weight[i][j] = _o_weight_save[i][j];
}

#endif


void save_BPnet(char* filename)
{
	int Innum = InLayerNodesNum;
	int Midnum = MidLayerNodesNum;
	int Outnum = OutLayerNodesNum;
	FILE *fp;//文件指针
    int i, d;
 
    /*文件的打开*/
    fp=fopen(filename,"w");//fopen打开文件，这个文件可以是当前不存在的。“w”以写入的形式打开，“r”以读的形式打开
    if(fp==NULL) //判断如果文件指针为空
    {
        printf("File cannot open! " );
        exit(0);//在以0的形式退出，必须在文件开头有#include <stdlib.h>,stdlib 头文件即standard library标准库头文件
    }
     //写入东西 保存为 .h文件
	
	fprintf(fp,"double _m_w_rate_save = %f;\n", _m_w_rate);//写入输入层-隐含层权值学习率
	fprintf(fp,"double _o_w_rate_save = %f;\n", _o_w_rate);//隐含层-网络层权学习率
	fprintf(fp,"double _m_t_rate_save = %f;\n", _m_t_rate);//输入层-隐含层阈值学习率
	fprintf(fp,"double _o_t_rate_save = %f;\n", _o_t_rate);//隐含层-网络层阈值学习率
	fprintf(fp,"int _it_nums_save = %d;\n", _it_nums); //写入迭代次数
	fprintf(fp,"double _err_save = %f;\n", _err);    //写入误差限
	fprintf(fp,"double _mint_save = %f;\n", _mint);    //写入归一化公用参数
	fprintf(fp,"double _maxt_save = %f;\n", _maxt); 
	//写入 输入层-隐含层阈值
	fprintf(fp,"double _m_threshold_save[%d] = {",MidLayerNodesNum);
	for(int i = 0;i < Midnum;i++){
		fprintf(fp,"%f", _m_threshold[i]);
			if(i == Midnum-1) 	
				continue;		
			fprintf(fp,",");
	}
	fprintf(fp,"};\n");
	//写入 隐含层-网络层阈值
	fprintf(fp,"double _o_threshold_save[%d] = {",OutLayerNodesNum);
	for(int i = 0;i < Outnum;i++){
		fprintf(fp,"%f", _o_threshold[i]);
			if(i == Outnum-1) 	
				continue;		
			fprintf(fp,",");
	}
	fprintf(fp,"};\n");
	//写入 输入层-隐含层权值
	fprintf(fp,"double _m_weight_save[%d][%d] = {",InLayerNodesNum,MidLayerNodesNum);
	for(int i = 0;i < Innum;i++)
		for(int j=0;j< Midnum;j++){
			fprintf(fp,"%f", _m_weight[i][j]);
			if((i == Innum-1) &&(j == Midnum-1))	
				continue;		
			fprintf(fp,",");		
		}
	fprintf(fp,"};\n");
	//写入 隐含层-网络层权值
	fprintf(fp,"double _o_weight_save[%d][%d] = {",MidLayerNodesNum,OutLayerNodesNum);
	for(int i = 0;i < Midnum;i++)
		for(int j=0;j< Outnum;j++){
			fprintf(fp,"%f", _o_weight[i][j]);	
			if((i == Midnum-1) &&(j == Outnum-1))	
				continue;		
			fprintf(fp,",");
		}
	fprintf(fp,"};\n");
     //关闭文件
    fclose(fp);
}

	
int  main()
{

	double a[N][InLayerNodesNum];
	double a2[n][InLayerNodesNum];
	double b[N][OutLayerNodesNum];
	double b2[n][OutLayerNodesNum];
/*  sample.h 样本处理  N = 120 n = 30      4  6   1
	int i_1 = 0, i_2 = 0;
	for (int count = 1; count < 4; count++)
	{

		for (int i = 50 * (count - 1); i < (50 * count - 10); i++) {
			for (int j = 0; j < 4; j++) {
				a[i_1][j] = Iris_Flowers[i][j];

			}
			b[i_1][0] = Iris_Flowers[i][4];
		
			i_1++;
		}
		for (int i = (50 * count - 10); i < 50 * count; i++) {
			for (int j = 0; j < 4; j++) {
				a2[i_2][j] = Iris_Flowers[i][j];
			}
			i_2++;
		}
	}
*/

//	test_data.h 中样本处理  N = 1000 n = 200； 3   20   3
	printf("load data...   wait...\n");
	printf("train data:%d  test data:%d\n",N,n);
	double std_b2[n][OutLayerNodesNum];

	for (int count = 0; count < N; count++)	{
		for (int i = 0; i < 3; i++){
			a[count][i] = train_data[count][i];
		}
		for (int j = 3; j < 6; j++){
			b[count][j] = train_data[count][j];
		}
	}
	for (int count = 0; count < n; count++)	{
		for (int i = 0; i < 3; i++){
			a2[count][i] = test_data[count][i];
		}
		for (int j = 3; j < 6; j++){
			std_b2[count][j] = test_data[count][j];
			printf("%f,",std_b2[count][j]);
		}
		printf("\n");
	}



#if TRAINNING 
	printf("trainning...   wait...\n");
	printf("in:%d  mid:%d  out:%d\n",InLayerNodesNum,MidLayerNodesNum,OutLayerNodesNum);
	BpNet_Init();
	BPtrain(a, b, N);
	save_BPnet("bpnet.h");
#elif TRAINNING==0
	load_BPnet();                 //不训练就下载网络
#endif
	BPsim(a2, b2, n);
	
	double err = 0;
	double errerr = 0.0; 
	double sum_errerr = 0.0;

	//计算误差
	for(int i = 0;i<n;i++){
		for(int j=0;j<OutLayerNodesNum;j++){
			err = b2[i][j]-std_b2[i][j];
			printf("%f   ",err);
			errerr += err*err;
		}
		errerr /= 3;
		printf("\n  %f\n ",errerr);
		sum_errerr += errerr;
		errerr = 0.0;
	}
	printf("sun_errerr= %f;   arrage_errerr = %f\n",sum_errerr,sum_errerr/n);
	return 1;
}


void normalize(double **t, int size)
	{//归一化处理
		_mint = 0x7FFFFFFF;//这里利用了“魔数”进行处理，不是最好的方法
		_maxt = -0x7FFFFFFF;

		//找到最大和最小值
		for (int i = 0; i<size; ++i)
		{
			for (int j = 0; j<OutLayerNodesNum; ++j)
			{
				if (_mint>t[i][j])
					_mint = t[i][j];
				if (_maxt<t[i][j])
					_maxt = t[i][j];
			}
		}

		if (_mint == _maxt)
			_mint = 0;

		double range = _maxt - _mint;

		//归一化
		//找到最大和最小值
		for (int i = 0; i<size; ++i)
		{
			for (int j = 0; j < OutLayerNodesNum; ++j)
				t[i][j] = (t[i][j] - _mint) / range;
		}
}

void unnormalize(double **t, int size)
{//反归一化处理
		double range = _maxt - _mint;

		//归一化
		//找到最大和最小值
		for (int i = 0; i<size; ++i)
		{
			for (int j = 0; j<OutLayerNodesNum; ++j)
				t[i][j] = t[i][j] * range + _mint;
		}
	}

void BpNet_Init(void)
{
		_m_w_rate = 0.5;//输入层-隐含层权值学习率
		_o_w_rate = 0.5;//隐含层-网络层权值学习率
		_m_t_rate = 0.5;//输入层-隐含层权值学习率
		_o_t_rate = 0.5;//隐含层-网络层权值学习率

		_it_nums = 10000;//迭代次数
		_err = 0.01;//误差限  0.00001

		//Get_Random(500);   //硬件adc随机产生种子			      
		//srand(rand_seed); //初始化成随机数

		srand((int)time(NULL)); 
		
		for (int i = 0; i<InLayerNodesNum; ++i)
		{
			for (int j = 0; j<MidLayerNodesNum; ++j)
				_m_weight[i][j] = (2.0*(double)rand() / RAND_MAX) - 1;
		}

		for (int i = 0; i<MidLayerNodesNum; ++i)
		{
			for (int j = 0; j<OutLayerNodesNum; ++j)
				_o_weight[i][j] = (2.0*(double)rand() / RAND_MAX) - 1;
		}
		for (int i = 0; i<MidLayerNodesNum; ++i)
			_m_threshold[i] = (2.0*(double)rand() / RAND_MAX) - 1;
		for (int i = 0; i<OutLayerNodesNum; ++i)
			_o_threshold[i] = (2.0*(double)rand() / RAND_MAX) - 1;

}

int BPtrain(double(*p)[InLayerNodesNum],double(*t)[OutLayerNodesNum], int size)
{//
		double o1[MidLayerNodesNum];
		double o2[OutLayerNodesNum];
		double error1[MidLayerNodesNum];
		double error2[OutLayerNodesNum];
		double max_error = 0;//记录最大误差
    //动态内存
		double *perr = (double *)malloc(size * sizeof(double));
		double **t_temp = (double **)malloc(size * sizeof(double));
		
		for (int i = 0; i<size; ++i)
		{
			t_temp[i] = (double *)malloc(OutLayerNodesNum * sizeof(double));
			for (int j = 0; j < OutLayerNodesNum; ++j)
				t_temp[i][j] = t[i][j];
		}

		normalize(t_temp, size);//归一化处理

		int i = 0;
		for (; i<_it_nums; ++i)
		{
			for (int j = 0; j<size; ++j)
			{
				//正向传播
				//计算隐含层输出
				for (int k = 0; k<MidLayerNodesNum; ++k)
				{
					o1[k] = _m_weight[0][k] * p[j][0];
					for (int m = 1; m<InLayerNodesNum; ++m)
						o1[k] += _m_weight[m][k] * p[j][m];
					//激励函数
					o1[k] = 1.0 / (1 + exp(-o1[k] - _m_threshold[k]));//隐含层各单元的输出
				}
				//计算输出层输出
				for (int k = 0; k<OutLayerNodesNum; ++k)
				{
					o2[k] = _o_weight[0][k] * o1[0];
					for (int m = 1; m<MidLayerNodesNum; ++m)
						o2[k] += _o_weight[m][k] * o1[m];
					//激励函数
					o2[k] = 1.0 / (1 + exp(-o2[k] - _o_threshold[k]));//隐含层各单元的输出 
				}

				//反向传播
				for (int k = 0; k<OutLayerNodesNum; ++k)
				{
					//计算输出层误差
					error2[k] = (t_temp[j][k] - o2[k])*o2[k] * (1 - o2[k]);
					//调整权值
					for (int m = 0; m<MidLayerNodesNum; ++m)
						_o_weight[m][k] += _o_w_rate * error2[k] * o1[m];
				}

				for (int k = 0; k<MidLayerNodesNum; ++k)
				{
					//计算隐含层误差
					double d = 0;
					for (int m = 0; m<OutLayerNodesNum; ++m)
						d += error2[m] * _o_weight[k][m];
					//调整权值
					error1[k] = d * o1[k] * (1 - o1[k]);

					for (int m = 0; m<InLayerNodesNum; ++m)
						_m_weight[m][k] += _m_w_rate * error1[k] * p[j][m];
				}

				double e = 0;
				for (int k = 0; k<OutLayerNodesNum; ++k)
					e += fabs(t_temp[j][k] - o2[k])*fabs(t_temp[j][k] - o2[k]);

				perr[j] = e / 2;
				//更新阈值
				for (int k = 0; k<OutLayerNodesNum; k++)
					_o_threshold[k] += _o_t_rate * error2[k]; //下一次的隐含层和输出层之间的新阈值  
				for (int k = 0; k<MidLayerNodesNum; k++)
					_m_threshold[k] += _m_t_rate * error1[k]; //下一次的输入层和隐含层之间的新阈值  
			}

			max_error = perr[0];
			for (int j = 1; j<size; ++j)
				if (perr[j]>max_error)
					max_error = perr[j];
			if (max_error<_err)
			{ 
//				printf("次数：%d\n",i);
				break;
			}
		}
		free(perr);
		for (int i = 0; i < size; ++i)
			free(t_temp[i]);
		free(t_temp);     //内存释放
	
		
		if (i >= _it_nums)
			return 0;
		return 1;
}

int BPsim(double(*p)[InLayerNodesNum], double(*t)[OutLayerNodesNum], int size)
{//
	
		double o1[MidLayerNodesNum];

		double **t_temp = (double **)malloc(size * sizeof(double));//保存t的数据
		for (int i = 0; i < size; ++i)
			t_temp[i] = (double *)malloc(OutLayerNodesNum * sizeof(double));

		for (int i = 0; i < size; ++i)
		{
			//正向传播
			//计算隐含层输出
			for (int k = 0; k < MidLayerNodesNum; ++k)
			{
				o1[k] = _m_weight[0][k] * p[i][0];
				for (int m = 1; m < InLayerNodesNum; ++m)
					o1[k] += _m_weight[m][k] * p[i][m];
				o1[k] = 1.0 / (1.0 + exp(-o1[k] - _m_threshold[k]));//隐含层各单元的输出 
			}
			//计算输出层输出
			for (int k = 0; k < OutLayerNodesNum; ++k)
			{
				t_temp[i][k] = _o_weight[0][k] * o1[0];
				for (int m = 1; m < MidLayerNodesNum; ++m)
					t_temp[i][k] += _o_weight[m][k] * o1[m];
				t_temp[i][k] = 1.0 / (1.0 + exp(-t_temp[i][k] - _o_threshold[k]));//隐含层各单元的输出 
			}
		}
		unnormalize(t_temp, size);         //反归一化
		for (int i = 0; i<size; ++i)
			for (int j = 0; j < OutLayerNodesNum; ++j) {
				t[i][j] = t_temp[i][j];
			}
		for (int i = 0; i < size; ++i)
			free(t_temp[i]);	
		free(t_temp);                           //内存释放
		return 1;
}

