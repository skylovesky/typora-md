

# 概念

## 加载因子

表示某个阀值，用0~1之间的小数来表示，当已有元素占比达到这个阀值后，底层将进行扩容操作

## 扩容方式

即指定每次扩容后的大小的规则，比如翻倍等

> 当底层实现涉及到扩容时，容器或重新分配一段更大的连续内存（如果是离散分配则不需要重新分配，离散分配都是插入新元素时动态分配内存），要将容器原来的数据全部复制到新的内存上，这无疑使效率大大降低。

## Hash

Hash (散列  或  哈希)是把任意长度的输入（又叫做预映射pre-image）通过散列算法（即哈希算法）变换成固定长度的输出

该输出就是散列值（哈希值）

### 特点

- 通过哈希值不能反向推导出原始数据

- 相同的数据得到完全相同的哈希值，不同的数据（即使细微差别）得到不同的哈希值

- 哈希算法的执行效率高，即使很长的文本也能很快的算出哈希值

- hash算法的冲突概率要小

  hash值的空间远远小于输入的空间，根据`抽屉原理`[^1]，一定会存在不同的输入被映射为相同的输出。

# 数组、链表、散列

![数组列表和散列](images/collection1.png)



# List

- 有序的

- 可重复

## Vector

线程安全，但速度慢

底层数据结构是数组结构

加载因子为1：即当 元素个数 超过 容量长度 时，进行扩容扩容增量：原容量的 1倍

## ArrayList

线程不安全，查询速度快

底层数据结构是数组结构

加载因子为1：即当元素个数超过容量长度时，进行扩容

扩容增量0.5：原容量的 0.5倍

1. new的本质是new 了一个空的Object数组

   ```java
   transient Object[] elementData;
   private static final Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};
   
   public ArrayList() {
       this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
   }
   ```

2. add(E e) ；

   - 并非线程安全；没见看到任何加锁的机制
   - 初始Object数组为空，添加一个后，Object数组为长度为10，之后每次超出上限，扩容长度都为上次的一半
   - 最后一次扩容为8个长度，再add就会抛出异常， 参照hugeCapacity（）；
   - Object[]最大长度为Integer.MAX_VALUE=2**31-1

   ```java
   protected transient int modCount = 0;//来自于父接口AbstractList的计数器
   private static final int MAX_ARRAY_SIZE = Integer.MAX_VALUE - 8;//最大长度  2**31-1
   private static final int DEFAULT_CAPACITY = 10;
   private int size;
   public boolean add(E e) {
           ensureCapacityInternal(size + 1);  //1.保证内部数组的长度是可以size++的
           elementData[size++] = e;
           return true;
   }
   private void ensureCapacityInternal(int minCapacity) {
           if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {//2.只有刚初始化的时候相等
               minCapacity = Math.max(DEFAULT_CAPACITY, minCapacity);//3.求得最大值 赋予容器的最小长度 初始化的时候DEFAULT_CAPACITY=10，minCapacity=size+1=1
           }
   
           ensureExplicitCapacity(minCapacity);//保证扩展能力 
   }
   
   private void ensureExplicitCapacity(int minCapacity) {
           modCount++;//计数器+1
   
           // overflow-conscious code
           if (minCapacity - elementData.length > 0)//4.初始化的时候
               grow(minCapacity);
   }
   
     private void grow(int minCapacity) {
           // overflow-conscious code
           int oldCapacity = elementData.length;
           int newCapacity = oldCapacity + (oldCapacity >> 1);//每次取值增加一半长度
           if (newCapacity - minCapacity < 0)
               newCapacity = minCapacity;
           if (newCapacity - MAX_ARRAY_SIZE > 0)//最大长度  
               newCapacity = hugeCapacity(minCapacity);//要么抛异常  要么只能再增加8个长度
           // minCapacity is usually close to size, so this is a win:
           elementData = Arrays.copyOf(elementData, newCapacity);//
   }
   
   private static int hugeCapacity(int minCapacity) {
           if (minCapacity < 0) // overflow
               throw new OutOfMemoryError();
           return (minCapacity > MAX_ARRAY_SIZE) ?
               Integer.MAX_VALUE :
               MAX_ARRAY_SIZE;
   }
   ```

   

# Set

- 元素无序的
- 不可重复

## HashSet

线程不安全，存取速度快

底层实现是一个HashMap（保存数据），实现Set接口

默认初始容量为16（为何是16，见下方对HashMap的描述）

加载因子为0.75：即当 元素个数 超过 容量长度的0.75倍 时，进行扩容

扩容增量：原容量的 1 倍    HashSet的容量为16，一次扩容后是容量为32



# Map

- 双列集合


## HashMap

线程不安全

默认初始容量为16 为何是16：16是2^4，可以提高查询效率，另外，32=16<<1       -->至于详细的原因可另行分析，或分析源代码）

加载因子为0.75：即当 元素个数 超过 容量长度的0.75倍 时，进行扩容

扩容增量：原容量的 1 倍 如 HashSet的容量为16，一次扩容后是容量为32

**在new HashMap的时候 初始化了一个算法的参数**

```java
final float loadFactor;
static final float DEFAULT_LOAD_FACTOR = 0.75f;
public HashMap() {
    this.loadFactor = DEFAULT_LOAD_FACTOR; 
}
```



### put(K key, V value) && putVal()



```java

transient Node<K,V>[] table;

public V put(K key, V value) {
    return putVal(hash(key), key, value, false, true);
}

static final int hash(Object key) {
    int h;
    return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);//>>>表示无符号右移16位
}

/**

*/
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
                   boolean evict) {
    //tab 引用当前HashMap的散列表
    //p 表示当前散列表的元素
    //n 表示散列表数组的长度
    //i 路由寻址的结果
    Node<K,V>[] tab; Node<K,V> p; int n, i;
    
    //延迟初始化的逻辑(最大限度的节省空间) 第一次调用putVal会初始化HashMap对象中的最耗内存的散列表
    if ((tab = table) == null || (n = tab.length) == 0)
        n = (tab = resize()).length;

    // (n-1)&hash 是 路由算法=(tab.length-1) & hash
    //最简单的一种情况: 寻址找到的桶位刚好是null,这时,直接将K,V ==> Node<K,V> 扔进去
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null);
    
    else {//路由寻址的找到的桶位已经有数据了，那就分好几情况 1.当前桶位的Key与要插入Node的Key相等  2.已经树化,成为红黑树  3.链表结构
        
        //e :不为null表示，找到一个与当前插入的K-V 中K一致的Node元素; 
        //k 临时
        Node<K,V> e; K k;
        
        //1.表示桶位中的Node元素,与当前插入的元素的Key完全一致，后续  需要进行替换操作 
        if (p.hash == hash &&((k = p.key) == key || (key != null && key.equals(k))))
            e = p;
        
        else if (p instanceof TreeNode)//2.
            e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
        else {//3.链表 ，而且链表的头与我们要插入的Key 不一致
            //循环遍历链表
            for (int binCount = 0; ; ++binCount) {
                if ((e = p.next) == null) {//找到了最后了,没有找到Key一致的元素,需要在链表尾巴加上node
                    p.next = newNode(hash, key, value, null);
                    
                    //是否需要树化 
                    if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                        //进行树化
                        treeifyBin(tab, hash);
                    break;
                }
                
                //条件成立 说明找到了Key一致的元素,   需要进行替换操作
                if (e.hash == hash &&
                    ((k = e.key) == key || (key != null && key.equals(k))))
                    break;
                
                //
                p = e;
            }
        }
        
        //进行替换操作
        if (e != null) { // existing mapping for key
            V oldValue = e.value;
            if (!onlyIfAbsent || oldValue == null)
                e.value = value;
            afterNodeAccess(e);
            return oldValue;
        }
    }
    
    //表示散列表结构修改次数， 替换不算，在替换动作中  直接返回了   return oldValue;
    ++modCount;
    
    //插入新元素，size自增，如果自增后的值大于扩容阈值，则触发扩容
    if (++size > threshold)
        resize();
    afterNodeInsertion(evict);
    return null;
}

```

```java

    /**
     * Basic hash bin node, used for most entries.  (See below for
     * TreeNode subclass, and in LinkedHashMap for its Entry subclass.)
     */
static class Node<K,V> implements Map.Entry<K,V> {
    final int hash;
    final K key;
    V value;
    Node<K,V> next;

    Node(int hash, K key, V value, Node<K,V> next) {
        this.hash = hash;
        this.key = key;
        this.value = value;
        this.next = next;
    }

    public final K getKey()        { return key; }
    public final V getValue()      { return value; }
    public final String toString() { return key + "=" + value; }

    public final int hashCode() {
        return Objects.hashCode(key) ^ Objects.hashCode(value);
    }

    public final V setValue(V newValue) {
        V oldValue = value;
        value = newValue;
        return oldValue;
    }

    public final boolean equals(Object o) {
        if (o == this)
            return true;
        if (o instanceof Map.Entry) {
            Map.Entry<?,?> e = (Map.Entry<?,?>)o;
            if (Objects.equals(key, e.getKey()) &&
                Objects.equals(value, e.getValue()))
                return true;
        }
        return false;
    }
}
```

### 扩容

```java
 /**
     * Initializes or doubles table size.  If null, allocates in
     * accord with initial capacity target held in field threshold.
     * Otherwise, because we are using power-of-two expansion, the
     * elements from each bin must either stay at same index, or move
     * with a power of two offset in the new table.
     *
     * @return the table
     */
final Node<K,V>[] resize() {
    
    	//引用扩容前的Hash表
        Node<K,V>[] oldTab = table;
    	//表示扩容前数组的长度
        int oldCap = (oldTab == null) ? 0 : oldTab.length;
    	//扩容之前的阈值 ，触发本次扩容的阈值
        int oldThr = threshold;
    
    	//扩容之后的数组长度,扩容之后再次触发扩容的阈值
        int newCap, newThr = 0;
    
    	//条件成立，说明hashMap中的散列表已经初始化过了，
        if (oldCap > 0) {
            //一次正常扩容
            
            if (oldCap >= MAXIMUM_CAPACITY) {
                threshold = Integer.MAX_VALUE;
                return oldTab;
            }
            else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                     oldCap >= DEFAULT_INITIAL_CAPACITY)
                newThr = oldThr << 1; // double threshold
        }
    
    	//oldCap==0
        else if (oldThr > 0) // initial capacity was placed in threshold
            newCap = oldThr;
        else {               // zero initial threshold signifies using defaults
            newCap = DEFAULT_INITIAL_CAPACITY;
            newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
        }
        if (newThr == 0) {
            float ft = (float)newCap * loadFactor;
            newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
                      (int)ft : Integer.MAX_VALUE);
        }
        threshold = newThr;
        @SuppressWarnings({"rawtypes","unchecked"})
            Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
        table = newTab;
        if (oldTab != null) {
            for (int j = 0; j < oldCap; ++j) {
                Node<K,V> e;
                if ((e = oldTab[j]) != null) {
                    oldTab[j] = null;
                    if (e.next == null)
                        newTab[e.hash & (newCap - 1)] = e;
                    else if (e instanceof TreeNode)
                        ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
                    else { // preserve order
                        Node<K,V> loHead = null, loTail = null;
                        Node<K,V> hiHead = null, hiTail = null;
                        Node<K,V> next;
                        do {
                            next = e.next;
                            if ((e.hash & oldCap) == 0) {
                                if (loTail == null)
                                    loHead = e;
                                else
                                    loTail.next = e;
                                loTail = e;
                            }
                            else {
                                if (hiTail == null)
                                    hiHead = e;
                                else
                                    hiTail.next = e;
                                hiTail = e;
                            }
                        } while ((e = next) != null);
                        if (loTail != null) {
                            loTail.next = null;
                            newTab[j] = loHead;
                        }
                        if (hiTail != null) {
                            hiTail.next = null;
                            newTab[j + oldCap] = hiHead;
                        }
                    }
                }
            }
        }
        return newTab;
    }
```

- HashMap 是一个散列表，它存储的内容是键值对(key-value)映射。
- HashMap 继承于AbstractMap，实现了Map、Cloneable、java.io.Serializable接口。
- HashMap 的实现不是同步的，这意味着它不是线程安全的。它的key、value都可以为null。此外，HashMap中的映射不是有序的。

### 扰动算法

```java
/**
	让key的hash值的高16位也参与路由运算
	之所以让高16位参与 是 增加散列性
*/
static final int hash(Object key) {
        int h;
        return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
}
```



## Hashtable

线程安全

默认初始容量为11

加载因子为0.75：即当 元素个数 超过 容量长度的0.75倍 时，进行扩容

扩容增量：原容量的 1 倍+1 如 Hashtable的容量为11，一次扩容后是容量为23



# 小结

| **Class** | **初始大小** | **加载因子** | **扩容倍数** | **底层实现**        | **Code**                                                     | **是否线程安全** | **同步方式** |
| --------- | ------------ | ------------ | ------------ | ------------------- | ------------------------------------------------------------ | ---------------- | :----------: |
| ArrayList | 10           | 1            | 1.5倍        | Object数组          | int newCapacity = oldCapacity + (oldCapacity >> 1); ">>"右移符号，所以是除以2，所以新的容量是就的1.5倍 Arrays.copyOf 调用 System.arraycopy 扩充数组 | 线程不安全       |      -       |
| Vector    | 10           | 1            | 2倍          | Object数组          | int newCapacity = oldCapacity + ((capacityIncrement > 0) ? capacityIncrement : oldCapacity); capacityIncrement默认为0，所以是加上本身，也就是2*oldCapacity，2倍大小 Arrays.copyOf 调用 System.arraycopy 扩充数组 | 线程安全         | synchronized |
| HashSet   | 16           | 0.75f        | 2倍          | HashMap<E,Object>   | add方法实际调用HashMap的方法put                              | 线程不安全       |      -       |
| HashMap   | 16           | 0.75f        | 2倍          | Map.Entry           | void addEntry(int hash, K key, V value, int bucketIndex) {     if ((size >= threshold) && (null != table[bucketIndex])) {         resize(2 * table.length);         hash = (null != key) ? hash(key) : 0;         bucketIndex = indexFor(hash, table.length);     }      createEntry(hash, key, value, bucketIndex); } | 线程不安全       |      -       |
| Hashtable | 11           | 0.75f        | 2倍+1        | Hashtable.Entry数组 | int newCapacity = (oldCapacity << 1) + 1; if (newCapacity - MAX_ARRAY_SIZE > 0) {     if (oldCapacity == MAX_ARRAY_SIZE)         return;     newCapacity = MAX_ARRAY_SIZE; } | 线程安全         | synchronized |





[^1]: 抽屉原理:桌上有十个苹果，要把这十个苹果放到九个抽屉里，无论怎样放，我们会发现至少会有一个抽屉里面放不少于两个苹果。如果每个抽屉代表一个集合，每一个苹果就可以代表一个元素，假如有n+1个元素放到n个集合中去，其中必定有一个集合里至少有两个元素