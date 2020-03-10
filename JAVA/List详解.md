# List



### ArrayList

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

   

# Map



### HashMap

1. 在new HashMap的时候 初始化了一个算法的参数

   ```java
   final float loadFactor;
   static final float DEFAULT_LOAD_FACTOR = 0.75f;
   public HashMap() {
       this.loadFactor = DEFAULT_LOAD_FACTOR; 
   }
   ```

2. put(K key, V value)

   ```java
   
   transient Node<K,V>[] table;
   
   public V put(K key, V value) {
       return putVal(hash(key), key, value, false, true);
   }
   
   static final int hash(Object key) {
       int h;
       return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);//>>>表示无符号右移16位
   }
   
   final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
                      boolean evict) {
       Node<K,V>[] tab; Node<K,V> p; int n, i;
       if ((tab = table) == null || (n = tab.length) == 0)
           n = (tab = resize()).length;
       if ((p = tab[i = (n - 1) & hash]) == null)
           tab[i] = newNode(hash, key, value, null);
       else {
           Node<K,V> e; K k;
           if (p.hash == hash &&
               ((k = p.key) == key || (key != null && key.equals(k))))
               e = p;
           else if (p instanceof TreeNode)
               e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
           else {
               for (int binCount = 0; ; ++binCount) {
                   if ((e = p.next) == null) {
                       p.next = newNode(hash, key, value, null);
                       if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                           treeifyBin(tab, hash);
                       break;
                   }
                   if (e.hash == hash &&
                       ((k = e.key) == key || (key != null && key.equals(k))))
                       break;
                   p = e;
               }
           }
           if (e != null) { // existing mapping for key
               V oldValue = e.value;
               if (!onlyIfAbsent || oldValue == null)
                   e.value = value;
               afterNodeAccess(e);
               return oldValue;
           }
       }
       ++modCount;
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
           Node<K,V>[] oldTab = table;
           int oldCap = (oldTab == null) ? 0 : oldTab.length;
           int oldThr = threshold;
           int newCap, newThr = 0;
           if (oldCap > 0) {
               if (oldCap >= MAXIMUM_CAPACITY) {
                   threshold = Integer.MAX_VALUE;
                   return oldTab;
               }
               else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                        oldCap >= DEFAULT_INITIAL_CAPACITY)
                   newThr = oldThr << 1; // double threshold
           }
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

   