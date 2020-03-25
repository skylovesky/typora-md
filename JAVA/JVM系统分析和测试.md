# JVM性能分析和测试

## jmap

JVM Memory Map命令用于生成heap dump文件，如果不使用这个命令，还可以使用-XX:+HeapDumpOnOutOfMemoryError参数来让虚拟机出现OOM的时候自动生成dump文件。 jmap不仅能生成dump文件，还可以查询finalize执行队列、Java堆和永久代的详细信息，如当前使用率、当前使用的是哪种收集器等。

##### 命令格式

- jmap [ option ] pid
- jmap [ option ] executable core
- jmap [ option ] [server-``id``@]remote-``hostname``-or-IP

##### 参数

option：选项参数，不可同时使用多个选项参数

pid：java进程id，命令ps -ef | grep java获取

executable：产生核心dump的java可执行文件

core：需要打印配置信息的核心文件

remote-hostname-or-ip：远程调试的主机名或ip

server-id：可选的唯一id，如果相同的远程主机上运行了多台调试服务器，用此选项参数标识服务器

##### options参数

heap : 显示Java堆详细信息

histo : 显示堆中对象的统计信息

permstat :Java堆内存的永久保存区域的类加载器的统计信息

finalizerinfo : 显示在F-Queue队列等待Finalizer线程执行finalizer方法的对象
dump : 生成堆转储快照
F : 当-dump没有响应时，强制生成dump快照



```cmake

C:\Users\Administrator>jmap -heap 5032
Attaching to process ID 5032, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 24.65-b04

using thread-local object allocation.
Parallel GC with 2 thread(s)

Heap Configuration:
   MinHeapFreeRatio = 0
   MaxHeapFreeRatio = 100
   MaxHeapSize      = 1073741824 (1024.0MB)
   NewSize          = 1310720 (1.25MB)
   MaxNewSize       = 17592186044415 MB
   OldSize          = 5439488 (5.1875MB)
   NewRatio         = 2
   SurvivorRatio    = 8
   PermSize         = 21757952 (20.75MB)
   MaxPermSize      = 85983232 (82.0MB)
   G1HeapRegionSize = 0 (0.0MB)

Heap Usage:
PS Young Generation
Eden Space:
   capacity = 135790592 (129.5MB)
   used     = 44056304 (42.01536560058594MB)
   free     = 91734288 (87.48463439941406MB)
   32.444297761070224% used
From Space:
   capacity = 3145728 (3.0MB)
   used     = 2892560 (2.7585601806640625MB)
   free     = 253168 (0.2414398193359375MB)
   91.95200602213542% used
To Space:
   capacity = 15204352 (14.5MB)
   used     = 0 (0.0MB)
   free     = 15204352 (14.5MB)
   0.0% used
PS Old Generation
   capacity = 90177536 (86.0MB)
   used     = 58725608 (56.005104064941406MB)
   free     = 31451928 (29.994895935058594MB)
   65.12221402900164% used
PS Perm Generation
   capacity = 49807360 (47.5MB)
   used     = 49629088 (47.329986572265625MB)
   free     = 178272 (0.170013427734375MB)
   99.64207699424342% used

25399 interned Strings occupying 2933336 bytes.
```

