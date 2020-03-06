# JUC（java.util.concurrent）并发



[java.util.concurrent](http://www.matools.com/file/manual/jdk_api_1.8_google/java/util/concurrent/package-frame.html)

[java.util.concurrent.atomic](http://www.matools.com/file/manual/jdk_api_1.8_google/java/util/concurrent/atomic/package-frame.html)

[java.util.concurrent.locks](http://www.matools.com/file/manual/jdk_api_1.8_google/java/util/concurrent/locks/package-frame.html)





##### 线程的状态

```java
public static enum Thread.State
extends Enum<Thread.State>{
    
}
```

线程状态。线程可以处于以下状态之一：

-  [`NEW`](http://www.matools.com/file/manual/jdk_api_1.8_google/java/lang/Thread.State.html#NEW) 
   尚未启动的线程处于此状态。 
-  [`RUNNABLE`](http://www.matools.com/file/manual/jdk_api_1.8_google/java/lang/Thread.State.html#RUNNABLE) 
   在Java虚拟机中执行的线程处于此状态。 
-  [`BLOCKED`](http://www.matools.com/file/manual/jdk_api_1.8_google/java/lang/Thread.State.html#BLOCKED) 
   被阻塞等待监视器锁定的线程处于此状态。 
-  [`WAITING`](http://www.matools.com/file/manual/jdk_api_1.8_google/java/lang/Thread.State.html#WAITING) 
   正在等待另一个线程执行特定动作的线程处于此状态。 
-  [`TIMED_WAITING`](http://www.matools.com/file/manual/jdk_api_1.8_google/java/lang/Thread.State.html#TIMED_WAITING) 
   正在等待另一个线程执行动作达到指定等待时间的线程处于此状态。 
-  [`TERMINATED`](http://www.matools.com/file/manual/jdk_api_1.8_google/java/lang/Thread.State.html#TERMINATED) 
   已退出的线程处于此状态。 

##### 实例

```java

class Ticket {

    private int number = 30;

    Lock lock = new ReentrantLock();//锁解接口  可重复锁的实现类  

    public void sale() {
        lock.lock();//加锁
        try {
            if (number > 0) {
                System.out.println(Thread.currentThread().getName() + "卖票第" + (number--) + "张票;剩余:" + number);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            lock.unlock();//释放锁
        }

    }

}

public class App {
    
    public static void main(String[] args) {
        Ticket ticket = new Ticket();

        new Thread(() -> {
            for (int i = 0; i < 50; i++) {
                ticket.sale();
            }
        }, "A").start();//别忘了start启动线程
        new Thread(() -> {
            for (int i = 0; i < 50; i++) {
                ticket.sale();
            }
        }, "B").start();
        new Thread(() -> {
            for (int i = 0; i < 50; i++) {
                ticket.sale();
            }
        }, "C").start();

    }

}
```



##### 思路：

线程  操作  资源类



#### 知识

##### lock接口

##### 以及lock接口实现类

