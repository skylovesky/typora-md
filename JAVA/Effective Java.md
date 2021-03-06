# Effective Java

## 第二章、创建和销毁对象

### 第1条：用静态工厂方法代替构造器

静态工厂方法与设计模式中的工厂方法（Factory Method）不同。

#### 优点

1. **静态工厂方法与构造器不同的第一大优势在于，它们有名称。**
   - 构造器缺点：一个类只能有一个带有指定签名的构造器，通过提供多个构造器，不同的参数列表。这并不是一个好的API，用户不知道该用哪个构造器。
   - 静态工厂方法优点：不受上述限制，当一个类需要多个带有签名的构造器时，用静态方法代替构造器（提供不同名称）。
2. **静态工厂方法与构造器不同的第二大优势在于，不必每次调用它们的时候都创建一个新的对象。**
   - 类似于享元(Flyweight)模式，避免创建不必要的重复对象。
   - 能够为重复的调用返回相同的对象，这样有助于类总能严格控制的某个时刻哪些实例应该存在，实例受控的类（instance-controlled）。
     - instance-controlled使得类可以确保它是一个 Singleton或者是不可实例化的。
     - instance-controlled使得不可变值类可以确保不会存在两个相等的实例，即当且仅当`a==b`时，`a.equals(b)`才为`ture`，这是享元模式的基础。
3. **静态工厂方法与构造器不同的第三大优势在于，它们可以返回原返回类型的任何子类型的对象。**
   - API可以返回对象，同时又不会使对象的类变成公有的（隐藏实现类会使API变得非常简洁）。这项技术适用于基于接口的框架。
4. **静态工厂方法与构造器不同的第四大优势在于，所返回对象的类可以随着每次调用而发生变化，这取决于静态工厂方法的参数值。**
   - 只要是已声明的返回类型的子类型，都是允许的。
5. **静态工厂方法与构造器不同的第五大优势在于，方法返回的对象所属的类，在编写包含该静态工厂方法类是可以不存在。**

#### 缺点

1. **静态工厂方法的主要缺点在于，类如果不含`public`或者`protected`的构造器，就不能被实例化。**

   因祸得福，鼓励程序员使用复合，而不是基础。

2. **静态工厂方法的第二个缺点在于，程序员很难发现它们。**

#### 静态工厂方法惯用名：

- **from**

  - 类型转换方法，只有单个参数，返回该类型的一个对应实例：

    Date  date = Date.from(instant);

- **of**

  - 聚合方法，多个参数，返回该类型的一个实例，把它们合并起来：

    Set<Color> colors = EnumSet.of(Color.PUROLE,Color.PINK);

- **valueOf**

- **instance**   or  **getInstance**

- **create**    or  **newInstance**

  确保每次调用返回一个新的实例

- **get*Type*** 在工厂方法出于不同的类中的时候使用。*Type* 表示工厂方法所返回的对象类型：

   FileStore fileStore = Files.getFileStore(Path);

- **new*Type***  在工厂方法出于不同的类中的时候使用。*Type* 表示工厂方法所返回的对象类型：

   BufferedReader br = Files.newBufferedReader(Path)；

- ***type***  get*Type*和new*Type*的简洁版：

   List list = Collections.list(data);

#### 总结

言而简之，静态工厂方法和公有构造器都各有用处，我们需要理解它们各自的长处。



### 第2条：遇到多个构造器参数是要考虑使用构造器模式（Builder）

#### 优点

可以有多个可变的参数。可以将多出调用某一个方法而传入的参数集中到一个域中。

#### 缺点

为了创建对象，必须先创建它的构造器。虽然创建构造器开销可能不明显，但在十分注重性能的情况下，可能就成问题了。

参照Builder设计模式



第3条：用私有构造器或者枚举类型强化Singleton属性