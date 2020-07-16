## 池

### 线程池

### 连接池

### 内存池

内存池在程序开始阶段，真正使用内存之前，先申请分配一定数量的、大小相等(一般情况下)的内存块留作备用。当有新的内存需求时，就从内存池中分出一部分内存块，若内存块不够再继续申请新的内存（程序一般有两个策略：1. 继续申请内存；2. GC回收）。 

#### 操作系统

![](images/gc1.png)

- R0:内核态
- R3:用户态
- 内存的数据是存在内存条上的，内存条是由操作系统借助驱动去操作的。
- 读写数据、分配内存等操作，是作用在内存上的，所以必然会出现从R3用户态，切入到R0内核态，然后再返回用户态。
- 物理地址是指处理器芯片发出，来进行地址空间寻址的地址，它与处理器地址引脚上发出的电信号相对应。 是处理器真实发送到其地址总线上的地址，而该地址应该访问到谁（ram controller？nand controller？），这个需要看该设备系统总线的仲裁，也就是总线仲裁，现在常用的总线仲裁有AXI AHBA等 。
-  线性地址（Linear Address）是逻辑地址到物理地址变换之间的中间层 。

#### 内存池结构

![](images/gc2.png)

- Memory Pool内存池，概念上的池子
- Memory Chunk内存块（操作系统块），
- Memory Cell



#### 对齐字节