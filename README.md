## SM3算法实现

### 代码概述：

sm3.cpp是SM3算法使用c语言的纯底层实现，头文件仅使用了stdio.h和string.h。开发平台为windows环境，编译器为Dev C++ 5.11。

### 算法思路

本文档中对算法本身原理不再说明，仅叙述本人实现思路（因此定义结构体的步骤会放在后面，这个顺序与代码构思过程一致）：

#### 1.常用布尔函数

首先进行的是压缩函数和置换函数的实现，如下图。为了编写方便把GG函数和FF函数按照j的适用情况分成了两个函数。此外还有消息扩展中经常用到的ROTL循环位移函数。事实上将这些函数写成宏定义的方式能进一步提高运行速度。

<img src="C:\Users\王彦森\AppData\Roaming\Typora\typora-user-images\image-20220730075502295.png" alt="image-20220730075502295" style="zoom:67%;" />

#### 2.消息扩展

使用上一步中的ROTL函数完成消息扩展并进行正确性测试（测试函数$testBiToW()$见源代码）

<img src="C:\Users\王彦森\AppData\Roaming\Typora\typora-user-images\image-20220730080216346.png" alt="image-20220730080216346" style="zoom: 67%;" />

<img src="C:\Users\王彦森\AppData\Roaming\Typora\typora-user-images\image-20220730080350503.png" alt="image-20220730080350503" style="zoom: 67%;" />

#### 3.消息块压缩

这里也是根据算法说明来写就行，包括CF()和Compress()两个函数。对于CF()函数，作用是单块消息压缩，拿一个数组V[]来存放哈希值，每次调用后对数组元素更新即可。Compress()函数是把整个消息扩展和消息块压缩的过程集成，便于后续调用。

<img src="C:\Users\王彦森\AppData\Roaming\Typora\typora-user-images\image-20220730085617243.png" alt="image-20220730085617243" style="zoom:67%;" />

<img src="C:\Users\王彦森\AppData\Roaming\Typora\typora-user-images\image-20220730085636267.png" alt="image-20220730085636267" style="zoom:67%;" />

#### 4.数据结构

在完成Compress()函数时，考虑到每次迭代需要一个记录哈希值的数组来传参给CF()函数，还需要一个存储当前块内容的数组，因此定义一个结构体sm3_context，其中包括以上提到的两个数组和一个记录总消息块数的int型变量total。

<img src="C:\Users\王彦森\AppData\Roaming\Typora\typora-user-images\image-20220730090139857.png" alt="image-20220730090139857" style="zoom:67%;" />

#### 5.sm3算法全过程

算法包括四部分：1.初始化 2.消息扩展 3.前n-1个消息块压缩迭代  4.最后一个消息快压缩迭代 。其中消息扩展放在Compress函数里。

<img src="C:\Users\王彦森\AppData\Roaming\Typora\typora-user-images\image-20220730090755701.png" alt="image-20220730090755701" style="zoom:67%;" />

#### 6.整理输出

需要将迭代后ctx中的哈希值（state数组）输出。由于state数组中的数据都是32位，因此需要用位运算拆成4个8位数放到output数组中，以便以16进制(printf函数的%X)或字符(printf函数的%c)形式输出。

<img src="C:\Users\王彦森\AppData\Roaming\Typora\typora-user-images\image-20220730091303556.png" alt="image-20220730091303556" style="zoom:67%;" />

#### 7.代码运行结果

输入任意消息直接运行即可。

![image-20220730091557609](C:\Users\王彦森\AppData\Roaming\Typora\typora-user-images\image-20220730091557609.png)