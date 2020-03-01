# SpringBoot 启动配置原理



###### 几个重要的事件回调机制

> **ApplicationContextInitlalizer**			配置在META-INF/spring.factories
>
> **SpringApplicationRunListener**			配置在META-INF/spring.factories
>
> **ApplicationRunner**				在IOC容器中
>
> **CommoandLineRunner**		在IOC容器中

![](images\canvas.png)

## 启动流程：

1. ### 创建SpringApplication对象

   ```java
   	public static ConfigurableApplicationContext run(Class<?>[] primarySources, String[] args) {
   		return new SpringApplication(primarySources).run(args);
   	}
   ```

   ```java
   	public SpringApplication(ResourceLoader resourceLoader, Class<?>... primarySources) {
   		this.resourceLoader = resourceLoader;
   		Assert.notNull(primarySources, "PrimarySources must not be null");
   		this.primarySources = new LinkedHashSet<>(Arrays.asList(primarySources));
   		this.webApplicationType = WebApplicationType.deduceFromClasspath();//webApplicatonType 枚举是web
           
           //在类路径下找到  SpringFactoriesLoader.FACTORIES_RESOURCE_LOCATION = "META-INF/spring.factories" 配置的所有ApplicationContextInitializer;然后保存起来  
   		setInitializers((Collection) getSpringFactoriesInstances(ApplicationContextInitializer.class));//注意
           
           //在类路径下找到  SpringFactoriesLoader.FACTORIES_RESOURCE_LOCATION = "META-INF/spring.factories" ApplicationListener;然后保存起来
   		setListeners((Collection) getSpringFactoriesInstances(ApplicationListener.class));//注意
           
           //从多个配置类中找到有main方法的主配置类
   		this.mainApplicationClass = deduceMainApplicationClass();
   	}
   ```

   ```java
   private Class<?> deduceMainApplicationClass() {
   		try {
   			StackTraceElement[] stackTrace = new RuntimeException().getStackTrace();
   			for (StackTraceElement stackTraceElement : stackTrace) {
   				if ("main".equals(stackTraceElement.getMethodName())) {
   					return Class.forName(stackTraceElement.getClassName());
   				}
   			}
   		}
   		catch (ClassNotFoundException ex) {
   			// Swallow and continue
   		}
   		return null;
   	}
   ```

   

2. ### 运行run方法

   ```java
   	public ConfigurableApplicationContext run(String... args) {
   		StopWatch stopWatch = new StopWatch();
   		stopWatch.start();
   		ConfigurableApplicationContext context = null;
   		Collection<SpringBootExceptionReporter> exceptionReporters = new ArrayList<>();
   		configureHeadlessProperty();
       
       	//获取SpringApplicationRunListeners  也是在类路径下META-INF/spring.factories获取的
   		SpringApplicationRunListeners listeners = getRunListeners(args);
           
           //回调所有的SpringApplicationRunListener.starting()方法
   		listeners.starting();
   		try {
               //封装命令行参数
   			ApplicationArguments applicationArguments = new DefaultApplicationArguments(args);
               
               //准备环境  
   			ConfigurableEnvironment environment = prepareEnvironment(listeners, applicationArguments);
               	//创建环境完成后 回调 listeners.environmentPrepared(environment);
               
               
   			configureIgnoreBeanInfo(environment);
               
               //打印图标 Spring boot
   			Banner printedBanner = printBanner(environment);
               
               //通过反射创建容器ApplicationContext 根据webApplicationType 决定创建什么类型的容器
   			context = createApplicationContext();
               
               //异常分析报告
   			exceptionReporters = getSpringFactoriesInstances(SpringBootExceptionReporter.class,
   					new Class[] { ConfigurableApplicationContext.class }, context);
               
               //准备上下文环境  将environment 保存到ioc中  
               //applyInitializers(context);回调之前保存的所有的 ApplicationContextInitializer.initialize(context);
               //listeners.contextPrepared(context);回调之前保存的所有的 ApplicationListener.contextPrepared(context);
   			prepareContext(context, environment, listeners, applicationArguments, printedBanner);
               //回调所有的ApplicationListener.contextLoaded(context)
               
               
               //刷新容器;IOC容器初始化，（如果是web应用还会创建嵌入式的Tomcat）加载IOC容器中的所有组件
               //扫描、创建、加载所有组件的地方（配置类、组件、自动配置）
   			refreshContext(context);
               
               //现在版本 该方法为空方法  在下面的callRunners(context, applicationArguments);
   			afterRefresh(context, applicationArguments);
   			stopWatch.stop();
   			if (this.logStartupInfo) {
   				new StartupInfoLogger(this.mainApplicationClass).logStarted(getApplicationLog(), stopWatch);
   			}
   			listeners.started(context);
               //从IOC容器中获取所有的ApplicationRunner和CommoandLineRunner进行回调；先回调ApplicationRunner
   			callRunners(context, applicationArguments);
   		}
   		catch (Throwable ex) {
   			handleRunFailure(context, ex, exceptionReporters, listeners);
   			throw new IllegalStateException(ex);
   		}
   
   		try {
   			listeners.running(context);
   		}
   		catch (Throwable ex) {
   			handleRunFailure(context, ex, exceptionReporters, null);
   			throw new IllegalStateException(ex);
   		}
           
           //
   		return context;
   	}
   ```

   ```java
   	protected ConfigurableApplicationContext createApplicationContext() {
   		Class<?> contextClass = this.applicationContextClass;
   		if (contextClass == null) {
   			try {
   				switch (this.webApplicationType) {
   				case SERVLET:
   					contextClass = Class.forName(DEFAULT_SERVLET_WEB_CONTEXT_CLASS);
   					break;
   				case REACTIVE:
   					contextClass = Class.forName(DEFAULT_REACTIVE_WEB_CONTEXT_CLASS);
   					break;
   				default:
   					contextClass = Class.forName(DEFAULT_CONTEXT_CLASS);
   				}
   			}
   			catch (ClassNotFoundException ex) {
   				throw new IllegalStateException(
   						"Unable create a default ApplicationContext, please specify an ApplicationContextClass", ex);
   			}
   		}
   		return (ConfigurableApplicationContext) BeanUtils.instantiateClass(contextClass);
   	}
   
   ```

   ```java
   	private void prepareContext(ConfigurableApplicationContext context, ConfigurableEnvironment environment,
   			SpringApplicationRunListeners listeners, ApplicationArguments applicationArguments, Banner printedBanner) {
   		context.setEnvironment(environment);
   		postProcessApplicationContext(context);
   		applyInitializers(context);
   		listeners.contextPrepared(context);
   		if (this.logStartupInfo) {
   			logStartupInfo(context.getParent() == null);
   			logStartupProfileInfo(context);
   		}
   		// Add boot specific singleton beans
   		ConfigurableListableBeanFactory beanFactory = context.getBeanFactory();
   		beanFactory.registerSingleton("springApplicationArguments", applicationArguments);
   		if (printedBanner != null) {
   			beanFactory.registerSingleton("springBootBanner", printedBanner);
   		}
   		if (beanFactory instanceof DefaultListableBeanFactory) {
   			((DefaultListableBeanFactory) beanFactory)
   					.setAllowBeanDefinitionOverriding(this.allowBeanDefinitionOverriding);
   		}
   		if (this.lazyInitialization) {
   			context.addBeanFactoryPostProcessor(new LazyInitializationBeanFactoryPostProcessor());
   		}
   		// Load the sources
   		Set<Object> sources = getAllSources();
   		Assert.notEmpty(sources, "Sources must not be empty");
   		load(context, sources.toArray(new Object[0]));
   		listeners.contextLoaded(context);
   	}
   ```

   