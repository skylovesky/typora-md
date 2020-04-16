# 设计模式

| **范围**/**目的** | **创建型模式Creational**                                    | **结构型模式**                                               | **行为型模式**                                               |
| ----------------- | ----------------------------------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **类模式**        | 工厂方法模式                                                | 适配器模式                                                   | 解释器模式<br />模板方法模式                                 |
| **对象模式**      | 抽象工厂模式<br /> 建造者模式 <br />原型模式 <br />单例模式 | 适配器模式 <br />桥接模式 <br />组合模式<br />装饰模式<br />外观模式 <br />享元模式 <br />代理模式 | 职责链模式 <br />命令模式 <br />迭代器模式 <br />中介者模式 <br />备忘录模式<br />观察者模式 <br />状态模式 <br />策略模式 <br />访问者模式 |



# 结构型模式

## 对象模式

###  门面 /外观模式（Facade）

#### 原理

>  **外部与一个子系统的通信必须通过一个统一的外观对象进行，使得子系统更易于使用** 

![](images/design/1.png)

#### 核心

门面模式的核心为Facade即门面对象，门面对象核心为几个点：

- 知道所有子角色的功能和责任
- 将客户端发来的请求委派到子系统中，没有实际业务逻辑
- 不参与子系统内业务逻辑的实现



#### 实例

##### slf4j日志

我们为什么要使用slf4j，举个例子：

```
我们自己的系统中使用了logback这个日志系统
我们的系统使用了A.jar，A.jar中使用的日志系统为log4j
我们的系统又使用了B.jar，B.jar中使用的日志系统为slf4j-simple

这样，我们的系统就不得不同时支持并维护logback、log4j、slf4j-simple三种日志框架，非常不便。
```

解决这个问题的方式就是引入一个适配层，由适配层决定使用哪一种日志系统，而调用端只需要做的事情就是打印日志而不需要关心如何打印日志，slf4j或者commons-logging就是这种适配层，slf4j是本文研究的对象。

从上面的描述，我们必须清楚地知道一点：**slf4j只是一个日志标准，并不是日志系统的具体实现**。理解这句话非常重要，slf4j只做两件事情：

- 提供日志接口
- 提供获取具体日志对象的方法

slf4j-simple、logback都是slf4j的具体实现，log4j并不直接实现slf4j，但是有专门的一层桥接slf4j-log4j12来实现slf4j。

为了更理解slf4j，我们先看例子，再读源码，相信读者朋友会对slf4j有更深刻的认识。



## 类模式

### 适配器模式（Adapter）

#### 原理

#### 实例

##### Tomcat

- 在Tomcat连接器组件中，连接器解析请求生成Request对象，而Catania容器接收ServletRequst对象，通过Adapter将Request转为ServletRequest。
- 同样的道里，将ServletResponse转为Response。



# 创建型模式

## 对象模式

### 构建器模式（Builder）



应用场景

- 对象创建过程比较复杂，或对创建顺序或组合有依赖(经典Builder模式，可参阅GOF《设计模式》)。
- 创建对象时所需参数较多，且包含较多可选参数(变种Builder模式，可参阅《Effective Java》构建器小节)

#### 代码

```java
public class NutritionFacts {

    private final int servingSize;

    private final int servings;

    private final int calories;

    /**
    * NutritionFacts的static内部类，可以通过NutritionFacts.Builder(..)进行调用，..取决于Builder的构造器
    */
    public static class Builder {

        private final int servingSize;

        private final int servings;

        private int calories = 0;

        public Builder(int servingSize, int servings) {
            this.servingSize = servingSize;
            this.servings = servings;
        }

        /**
        * 返回Builder本身this,多个这样的方法形成流式的API
        */
        public Builder calories(int value) {
            calories = value;
            return this;
        }

        /**
        * 最终通过Builder的this对象的属性，new NutritionFacts并返回
        */
        public NutritionFacts builder() {
            return new NutritionFacts(this);
        }
    }

    /**
    * 唯一一个私有构造器，通过Builder的属性进行赋值。
    */
    private NutritionFacts(Builder builder) {
        servings = builder.servings;
        servingSize = builder.servingSize;
        calories = builder.calories;
    }
    
}
```

```java
  NutritionFacts nutritionFacts = new NutritionFacts.Builder(230,20).calories(10).builder();
```



#### 原理

- 得到一个流式的API

## 类模式

# 行为型模式

## 对象模式

## 类模式





