# JUC（java.util.concurrent）并发

## 多线程

### 上联：线程  操作  资源类

### 下联：判断  干活  通知

- this.wait() or  not wait()   ----wait()匹配的是synchronized
- do something
- this.notifyAll() 唤醒通知，这里注意虚假唤醒

### 注意：警惕虚假唤醒

多线程横向调用要用while()

![](images\1.png)

```text
举例：
	一群人，坐飞机，过安检（判断），坐上飞机后，接到消息通知，飞机停飞，同志们都下了飞机，刚下飞机，迈出一步，接到通知，飞机可以准时起飞（刚才的消息是虚假通知）。
	问:这个时候再次登机，需不需要再次过安检（再次判断）。
	答:必须需要再次过安检。
	判断wait(),notifyAll()通知这种机制必须用while，苏醒之后必须拉回来重新判断;
	
```

```text
public final void wait()
                throws InterruptedException

导致当前线程等待，直到另一个线程调用该对象的notify()方法或notifyAll()方法。 换句话说，这个方法的行为就好像简单地执行呼叫wait(0) 。

当前的线程必须拥有该对象的显示器。 该线程释放此监视器的所有权，并等待另一个线程通知等待该对象监视器的线程通过调用notify方法或notifyAll方法notifyAll 。 然后线程等待，直到它可以重新获得监视器的所有权并恢复执行。

像在一个参数版本中，中断和虚假唤醒是可能的，并且该方法应该始终在循环中使用：

  synchronized (obj) {
         while (<condition does not hold>)
             obj.wait();
         ... // Perform action appropriate to condition
     } 

该方法只能由作为该对象的监视器的所有者的线程调用。
```





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



#### 知识

##### lock接口

##### 以及lock接口实现类

##### Condition与lock接口搭配



## 锁//to do

- 公平锁和非公平锁

#####  synchronized    wait

- synchronized 写在方法上，但作用到类上
- synchronized 在非静态方法上，作用的是new 的对象 synchronized（this）
- synchronized 在静态方法上，作用的是该类的Class对象（全局锁）