

# JAVA笔记

## new对象的创建过程

1. new指令 JVM首先对符号进行解析，如果找不到对应的符号引用，那么这个类还没有被加载，因此JVM进行**类加载**（//TO DO）。
2. 符号引用解析完毕之后，JVM会为对象在堆中分配内存。HotSpot虚拟机实现的JAVA对象包括三个部分：对象头、实例字段和对齐填充字段（填充8的倍数）。//TO DO
3. 为对象分配完堆内存之后，JVM会将该内存（除了对象头区域）进行零值初始化（半初始化）， 这也就解释了为什么JAVA的属性字段无需显示初始化就可以被使用，而方法的局部变量却必须要显示初始化后才可以访问。
4.  JVM会调用对象的构造函数，当然，调用顺序会一直上溯到Object类。 

## jdk1.8新特性

### 函数式接口

- @FunctionInterface   lambda表达式
- default 多个
- static多个



## 线程安全的List

```java
new Vector()
```

```java
Collections.synchronizedList(new ArrayList<>());
```

```java
Collections.synchronizedMap(new HashMap());
```

```java
Collections.synchronizedSet(new HashSet<>());
```



### 写实复制 Copy-on-write 

1. ```java
   Set set = new CopyOnWriteArraySet(new HashSet());
   ```

2. ```java
   List list = new CopyOnWriteArrayList(new ArrayList());
   ```

3. ```java
   Map<String,Object> map = new ConcurrentHashMap<>();
   ```

```java
java.util.concurrent;
public class CopyOnWriteArrayList<E>
    implements List<E>, RandomAccess, Cloneable, java.io.Serializable {
    
     public boolean addAll(int index, Collection<? extends E> c) {
        Object[] cs = c.toArray();
        final ReentrantLock lock = this.lock;
        lock.lock();
        try {
            Object[] elements = getArray();
            int len = elements.length;
            if (index > len || index < 0)
                throw new IndexOutOfBoundsException("Index: "+index+
                                                    ", Size: "+len);
            if (cs.length == 0)
                return false;
            int numMoved = len - index;
            Object[] newElements;
            if (numMoved == 0)
                newElements = Arrays.copyOf(elements, len + cs.length);
            else {
                newElements = new Object[len + cs.length];
                System.arraycopy(elements, 0, newElements, 0, index);
                System.arraycopy(elements, index,
                                 newElements, index + cs.length,
                                 numMoved);
            }
            System.arraycopy(cs, 0, newElements, index, cs.length);
            setArray(newElements);
            return true;
        } finally {
            lock.unlock();
        }
    }
}
```





## 关键字

### 一、volatile

当对**非**volatile变量进行读写的时候，每个线程先从主内存拷贝变量到CPU缓存中，如果计算机有多个CPU，每个线程可能在不同的CPU上被处理，这意味着每个线程可以拷贝到不同的CPU cache中。
volatile变量不会被缓存在寄存器或者对其他处理器不可见的地方，保证了每次读写变量都从主内存中读，跳过CPU cache这一步。**当一个线程修改了这个变量的值，新值对于其他线程是立即得知的**

#### 原理浅析

##### 禁止指令重排

指令重排序是JVM为了优化指令、提高程序运行效率，在不影响单线程程序执行结果的前提下，尽可能地提高并行度。指令重排序包括编译器重排序和运行时重排序。
在JDK1.5之后，可以**`使用volatile变量禁止指令重排序`**。针对volatile修饰的变量，在读写操作指令前后会插入内存屏障，指令重排序时不能把后面的指令重排序到内存屏

##### 指令重排带来的问题

```java
线程A中
{
    context = loadContext();
    inited = true;
}

线程B中
{
    if (inited) 
        fun(context);
}
```

 如果线程A中的指令发生了重排序，那么B中很可能就会拿到一个尚未初始化或尚未初始化完成的context,从而引发程序错误。 

##### 禁止指令重排的原理

olatile关键字提供内存屏障的方式来防止指令被重排，编译器在生成字节码文件时，会在指令序列中插入内存屏障来禁止特定类型的处理器重排序。

  JVM内存屏障插入策略：

1. 在每个volatile写操作的前面插入一个StoreStore屏障；
2. 在每个volatile写操作的后面插入一个StoreLoad屏障；
3. 在每个volatile读操作的后面插入一个LoadLoad屏障；
4. 在每个volatile读操作的后面插入一个LoadStore屏障。

##### 指令重排在双重锁定单例模式中的影响

 基于双重检验的单例模式(懒汉型) 

```java
public class Singleton3 {
    private static Singleton3 instance = null;

    private Singleton3() {}

    public static Singleton3 getInstance() {
        if (instance == null) {
            synchronized(Singleton3.class) {
                if (instance == null)
                    instance = new Singleton3();// 非原子操作
            }
        }

        return instance;
    }
}
```

instance= new Singleton()并不是一个原子操作，其实际上可以抽象为下面几条JVM指令：

```java
memory =allocate();    //1：分配对象的内存空间 
ctorInstance(memory);  //2：初始化对象 
instance =memory;     //3：设置instance指向刚分配的内存地址
```




上面操作2依赖于操作1，但是操作3并不依赖于操作2。所以JVM是可以针对它们进行指令的优化重排序的，经过重排序后如下：

```java
memory =allocate();    //1：分配对象的内存空间 
instance =memory;     //3：instance指向刚分配的内存地址，此时对象还未初始化
ctorInstance(memory);  //2：初始化对象
```

指令重排之后，instance指向分配好的内存放在了前面，而这段内存的初始化被排在了后面。在线程A执行这段赋值语句，在初始化分配对象之前就已经将其赋值给instance引用，恰好另一个线程进入方法判断instance引用不为null，然后就将其返回使用，导致出错。 

#####  **解决办法** 

 用volatile关键字修饰instance变量，使得instance在读、写操作前后都会插入内存屏障，避免重排序。

```java
public class Singleton3 {
    private static volatile Singleton3 instance = null;

    private Singleton3() {}

    public static Singleton3 getInstance() {
        if (instance == null) {
            synchronized(Singleton3.class) {
                if (instance == null)
                    instance = new Singleton3();
            }
        }
        return instance;
    }
}
```




 简单地说就是防止编译器对代码进行优化 ；

 编译器在用到这个变量时必须每次都小心地重新读取这个变量的值，而不是使用保存在寄存器里的备份 ；

1）并行设备的硬件寄存器（如：状态寄存器）

2）一个中断服务子程序中会访问到的非自动变量（Non-automatic variables)

3）多线程应用中被几个任务共享的变量

### 二、const

 被const修饰的对象在所在的作用域无法进行修改操作，编译器对于试图直接修改const对象的表达式会产生编译错误。 