# Spring 注解驱动

## IOC容器

##### @Configuration   

- 标识一个配置类  这是一个配置文件 
- annotation = new AnnotationConfigApplicationContext(Config.class);

##### @Bean    

- 给容器中注册一个bean  在 @Configuration  ;
- 返回值的类型就是注册到@Bean方法的返回值

##### @ComponentScan

- JDK1.8 可以重复添加@ComponentScan 写不同的过滤规则，不是JDK1.8可以用@ComponentScans

- 过滤规则为Filter数组

  ```java
  @ComponentScan(value = "com.free",excludeFilters = @ComponentScan.Filter(type = FilterType.ANNOTATION,classes = Controller.class),useDefaultFilters = false) 
  ```

   

- 包扫描 @Repository；@Service；@Controller；@Component；...
- excludeFilters=@Filter[] 指定排除规则
- includeFilters=@Filter[] 指定包含规则（要使该规则生效，需要禁用掉默认规则） 在 ComponentScan 中 有useDefaultFilters（默认过滤规则）  设置为 false（禁用掉默认规则）
- FilterType enum  过滤规则
  -  ANNOTATION			注解
  -  ASSIGNABLE_TYPE    指定的类型
  -  ASPECTJ  ASPECTJ 表达式
  -  REGEX 正则表达式
  -  CUSTOM 自定义规则
    -   Filter candidates using a given custom  {@link org.springframework.core.type.filter.TypeFilter} implementation. 

##### @ComponentScans

##### @Scope  组件作用域

- 默认单实例singleton
  - IOC容器启动创建对象并放在IOC容器中
- 多实例prototype
  - IOC容器并不会调用方法，不会创建对象
  - 每次获取bean时，调用方法，创建对象

##### @Lazy-bean懒加载

- 容器启动时并不会创建对象
- 第一次使用（或获取）时创建对象

##### @Conditional条件判断

- 可以表在类和方法上 @Target({ElementType.TYPE, ElementType.METHOD})

- 按照一定条件进行判断，满足条件给容器中注册bean

- 自定义的判断类必须实现Condition接口

- @Conditional中为 Condition 数组

  ```java
  @Retention(RetentionPolicy.RUNTIME)
  @Target({ElementType.TYPE, ElementType.METHOD})
  public @interface Conditional {
  	Class<? extends Condition>[] value();
  }
  ```

  ```java
  public interface Condition {
  	boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata);
  }
  ```

#### 注册组件的方式：

##### 1. 包扫描+组件注解

（@ComponentScan + @Controller/@Service/Repository/@Component）

##### 2.@Bean

##### 3.@Import 

- 快速给容器中导入注解

- ID默认是组件的全类名

- 多个组件 数组形式导入 @Import（{Color.class,Red.Class}）

- ImportSelector导入选择器:

  - 返回需要导入组件的全类名数组

  - 实现ImportSelector接口

  - selectImports(AnnotationMetedata metedata)抽象方法  AnnotationMetedata  当前类的注解信息，类信息

    ```java
    @import({Color.class,MySelectImport.class})
    ```

- ImportBeanDefinitionRegistrar

  - 实现ImportBeanDefinitionRegistrar接口
  - 调用BeanDefinitionRegistry.registerBeanDefinition()方法手动注册Bean

##### 4.FactoryBean（Spring）

- 实现FactoryBean<Bean>

- 方法 getObjectType() 返回对象类型  Bean.class

- 方法 isSingle（） 是否单例

- ```java
  @Bean
  public ColorFactoryBean createFactory(){
      return new ColorFactoryBean();
  }
  ```

  - 返回的是  Color的全类名，而不是ColorFactoryBean
  - 想要获取ColorFactoryBean本身,则需要加上前缀符号 &
    - getBean("&ColorFactoryBean") 获取的是ColorFactoryBean

#### Bean生命周期

bean创建-------初始化--------销毁的过程

由容器管理的

##### 第一种、自定义生命周期

- 创建对象（并不能自定义）
  - 单实例Bean是在容器启动的时候创建对象
  - 多实例Bean是在容器获取的时候创建对象
- 指定初始化
  - @Bean(initMethod="initMethod");initMethod是Bean组件中自定义的方法
  - 在对象创建完成，并赋值好，调用初始化方法
- 指定销毁
  - @Bean(destoryMethod="destoryMethod");destoryMethod是Bean组件中自定义的方法
  - 单实例Bean，容器关闭时，applicationContext.close();调用组件销毁方法；多实例bean容器不会调用销毁方法

##### 第二种、自定义生命周期

- 让Bean实现InitializingBean定义初始化逻辑
- 让Bean实现DisposableBean定义销毁逻辑（单实例Bean才有用）

##### 第三种、自定义生命周期

- JSR250
- @PostConstruct (构造完成后) 在bean创建完成，属性值赋值完成，执行该方法，标记在方法上
- @PreDestroy（销毁前） 在容器销毁bean之前，执行该方法，标记在方法上

##### 第四种、自定义生命周期

- BeanPostProcessor（Bean后置处理器）,在bean执行初始前后执行处理工作
- 自定义一个类，实现BeanPostProcessor，所有的bean初始化前和后都会进入到该方法执行
- 自定义类是一个组件，需要加入到容器中
  - postProcessBeforeInitialization()在创建**任何Bean**实例，任何初始化方法调用**之前**执行
  - postProcessAfterInitialization()在创建**任何Bean**实例，初始化方法调用**之后**执行

#### BeanPostProcessor工作原理

#### 属性赋值

##### @Value

- 在组件的属性上

- 基本数组
- 可以写SpELl
- 可以${} ；取出配置文件中值（在运行环境变量里面的值）

##### @PropertySource

- 读取外部配置文件
- 在类上

- （value={"classpath:/my.properties","...","..."}）

#### 自动装配

Spring利用依赖注入（DI），完成IOC容器中的各个组件的依赖赋值

##### @Autowired

- 默认优先按照类型去容器中寻找对应的组件，找到就赋值

- 如果找到多个相同的组件，再将属性的名称作为组件的ID去容器中查找

- 自动装配，默认一定要将属性赋值，默认一定要找到（required=true），否则报错（找不到组件）

  可以设置Autowired(required = false) 非必须找到

##### @Qualifier("name") 

- IOC容器中某个组件有多个时，可以明确指定需要装配组件的ID
- 和@Autowired配合使用

##### @Primary 

- 设置为默认首选装配的Bean（当IOC容器中该bean有多个时）
- 和@Bean搭配使用，装配的位置不能有@Qualifier





