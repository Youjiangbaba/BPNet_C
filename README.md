# bp-C-
C语言实现的BP神经网络，默认三层，自定义IN、MID、OUT的维度。
无其他依赖，可直接移植在单片机中。

sample.h 为鸢尾花卉数据集（150）。120训练，30测试； 4 6 1 分类准确率百分之九十五以上。
testdata.h 为自定义数据集。1000 训练， 200测试 ；3 20 3 误差平均方差 3000；  3 8 3 误差平均方差 2200；3 80 3 误差平均方差 4000


 *****************************************************************************
   *Modified 	              : 2019/1/4 by Jiang
   *Changes                   : add function : save and load net
				add testdata to test
   *Requires attention        : #define TRAINNING  1 : function train and save new net
				#define TRAINNING  0 : load existed net to sim
   				DL_bpbet.h to change LayerNodesNum 
				bp_net.h  to see the bp-nn-net
 *****************************************************************************

其他：
1、c语言注意头文件包含问题。（a.h包含b.h;c.c包含a.h、b.h则会出错）
2、c语言编译时遇到找不到 math.h 中的函数，加上  -lm 
