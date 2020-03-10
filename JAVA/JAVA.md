

# JAVA笔记



### jdk1.8新特性

- #### 函数式接口

  - @FunctionInterface
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





### volatile

