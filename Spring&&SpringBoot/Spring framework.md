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

##### @Lazy懒加载

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
- @Value(${}) 也可以作用于方法内的属性上 
  - public void test(@Value("$(password")  String  password )

##### @PropertySource

- 属性的来源

- 读取外部配置文件
- 作用于类
- 加载到了环境变量中ConfigurableEnvironment
- 读取外部配置文件中的 K-V保存到运行的环境变量中
- @PropertySource（value = {“classpath：/.properties”，“”}）
- 用@Value("${Key}")  取出配置文件的值

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

##### @Resource

- java规范的注解
- 默认按照组件名称装配的
- 没有能支持@Primary
- 和@Autowired配合

##### @Inject

- 需要导入javax.inject包
- 和Autowired功能一样

##### AutowiredAnnotationBeanPostProcessor

- 解析完成自动装配功能

##### @Autowired

- 标注在方法上，Spring容器在创建当前对象，就会调用方法，完成赋值，方法使用的参数，自定义类型的值会从IOC容器中获取
- 标记在有参构造器方法上（或者标记在构造器的参数前面），也是从IOC注入该参数
- 参数前面前，从IOC容器注入参数

##### @Bean

- 标注的方法，方法参数的值从IOC容器中获取

##### @Profile

- Profile是Spring提供的，可以根据当前环境，动态激活和切换一系列组件的功能

- 指定组件在哪个环境下才能被注册到IOC容器中

- @Profile("default")  default是默认环境标识

  1. 加上环境标识后，只有这个环境被激活，组件才会被加入到IOC容器中

  2. 默认激活的环境标识是default

  3. 激活环境

     - 使用命令行动态参数：JVM运行参数

       -Dspring.profiles.active=test  

       所有环境标识为test的被激活

     - 代码

       1. 创建ApplicationContext对象

          AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(); //无参构造器

       2. 设置需要激活的环境

          context.getEnvironment().setActiveProfiles("test","dev");

       3. 注册主配置类

          context.register(MainConfig.class);

       4. 启动刷新容器

          context.refresh();

- @Profie()在配置类上时，只有指定的环境的时候，整个配置类里面所有配置才能开始生效

- 没有标注环境标识的bean，在任何环境都会被加载

## AOP面向切面【动态代理】

指程序在运行期间动态的将某段代码切入到指定位置进行运行的编程方式

1. 导入AOP模块：Spring AOP(spring-aspects)
2. 在业务逻辑运行（方法之前、方法运行结束、方法出现异常）
3. 定义切面类；（切面类感知业务逻辑的运行）
   - 通知方法
     - 前置通知（@Before）：在目标方法运行之前通知
     - 后置通知（@After）：在目标方法运行之后通知
     - 返回通知（@AfterReturning）：在目标方法正常返回通知
     - 异常通知（@AfterThrowing）：在目标方法运行出现异常
     - 环绕通知（@Around）：动态代理，手动推进方法运行
4. 给切面类的目标方法标注何时何地运行（通知注解）
5. 将切面类和业务逻辑类（目标方法所在类）都切入到IOC容器中
6. 必须告诉Spring哪个类是切面类（给切面类加注解@Aspect）
7. 给配置类中Config 加入@EnableAspectJAutoProxy，开启基于注解的AOP模式

##### @EnableAspectJAutoProxy

- 开启基于注解的AOP模式
- 在Spring中很多@EnableXXX 开启某一项功能

##### @Aspect

- 标记这是一个切面类

##### JoinPoint类

- 

##### @Before

- 在目标方法之前切入
- @Before（“....”）  切入点表达式（指定在哪切入）

##### @After

- 在目标方法运行结束之后切入
- 无论方法是正常结束还是异常结束

##### @AfterReturning

```java
@AfterReturning(value="point()",returning="result")//returning 指定方法接收返回值
public void logReturn(JoinPoint joinPoint,Object result){//JoinPoint如果写，必须写在参数的第一位
    
}
```

##### @AfterThrowing

```java
@AfterThrowing(value="point()",throwing="exception")//throwing 指定方法接收异常
public void logException(JoinPoint joinPoint,Exception exception){
    
}
```



##### @Pointcut

- 抽取公共的切入点表达式

  ```java
  @Pointcut("execution( public int com.sky.*(..))")
  public void pointCut(){}
  
  @Before("pointCut()")
  public void logStart(){}
  ```




### AOP原理

##### @EnableAspectJAutoProxy

- 利用AspectJAutoProxyRegistrar 给容器注册Bean

  源码如下：

  ```java
  @Target(ElementType.TYPE)
  @Retention(RetentionPolicy.RUNTIME)
  @Documented
  @Import(AspectJAutoProxyRegistrar.class)
  public @interface EnableAspectJAutoProxy {
  	boolean proxyTargetClass() default false;
  	boolean exposeProxy() default false;
  }
  
  ```

  ```java
  class AspectJAutoProxyRegistrar implements ImportBeanDefinitionRegistrar {//ImportBeanDefinitionRegistrar注册组件接口;参考本文档@import
  		
  	@Override
  	public void registerBeanDefinitions(
  			AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
  
  		AopConfigUtils.registerAspectJAnnotationAutoProxyCreatorIfNecessary(registry);
  
  		AnnotationAttributes enableAspectJAutoProxy =
  				AnnotationConfigUtils.attributesFor(importingClassMetadata, EnableAspectJAutoProxy.class);
  		if (enableAspectJAutoProxy.getBoolean("proxyTargetClass")) {
  			AopConfigUtils.forceAutoProxyCreatorToUseClassProxying(registry);
  		}
  		if (enableAspectJAutoProxy.getBoolean("exposeProxy")) {
  			AopConfigUtils.forceAutoProxyCreatorToExposeProxy(registry);
  		}
  	}
  
  }
  ```

  ```java
  public abstract class AopConfigUtils {
      /**
  	 * The bean name of the internally managed auto-proxy creator.
  	 */
  	public static final String AUTO_PROXY_CREATOR_BEAN_NAME =
  			"org.springframework.aop.config.internalAutoProxyCreator";
      
      /**
  	 * Stores the auto proxy creator classes in escalation order.
  	 */
  	private static final List<Class<?>> APC_PRIORITY_LIST = new ArrayList<Class<?>>();
  
      static {
  		APC_PRIORITY_LIST.add(InfrastructureAdvisorAutoProxyCreator.class);
  		APC_PRIORITY_LIST.add(AspectJAwareAdvisorAutoProxyCreator.class);
  		APC_PRIORITY_LIST.add(AnnotationAwareAspectJAutoProxyCreator.class);
  	}
      
   		private static BeanDefinition registerOrEscalateApcAsRequired(Class<?> cls, BeanDefinitionRegistry registry, Object source) {
  		Assert.notNull(registry, "BeanDefinitionRegistry must not be null");
  		if (registry.containsBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME)) {
  			BeanDefinition apcDefinition = registry.getBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME);
  			if (!cls.getName().equals(apcDefinition.getBeanClassName())) {
  				int currentPriority = findPriorityForClass(apcDefinition.getBeanClassName());
  				int requiredPriority = findPriorityForClass(cls);
  				if (currentPriority < requiredPriority) {
  					apcDefinition.setBeanClassName(cls.getName());
  				}
  			}
  			return null;
  		}
  		RootBeanDefinition beanDefinition = new RootBeanDefinition(cls);
  		beanDefinition.setSource(source);
  		beanDefinition.getPropertyValues().add("order", Ordered.HIGHEST_PRECEDENCE);
  		beanDefinition.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);
  		registry.registerBeanDefinition(AUTO_PROXY_CREATOR_BEAN_NAME, beanDefinition);
  		return beanDefinition;
  	}   
  }
  ```

  给容器中注入AnnotationAwareAspectJAutoProxyCreator  （自动代理创建器）

##### AnnotationAwareAspectJAutoProxyCreator 

> **所有@EnableXXX的原理，类似，查看@Enable注册自动或初始化时了哪些组件，查看注册组件什么时候工作，以及工作时的功能**

继承关系

![](images/spring1.png)

> ##### 后置处理器
>
> （BeanPostProcessor）
>
> bean初始化前后做的事
>
> ##### 自动装配
>
> （BeanFactoryAware）

##### BeanPostProcessor 

- 主要查看其子类重写的实现方法 



##### BeanFactoryAware

- 主要查看其子类重写的实现方法



## 声明式事务

#### @Transactional

- 表示当前方法是事务方法

- 需要@EnableTransactionManagement
- 需要配置事务管理器PlatformTransactionManager的实现接口（基于平台的PlatformTransactionManager）

#### @EnableTransactionManagement

- 开启基于注解的事务管理功能

#### PlatformTransactionManager

- 事务管理器  需要配置要管理的数据源DataSource
- 注册PlatformTransactionManager到容器中



## 扩展原理





# refresh() 刷新容器

AbstractApplicationContext