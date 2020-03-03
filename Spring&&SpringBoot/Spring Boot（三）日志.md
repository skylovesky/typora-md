# 三、日志

## 1、日志框架

- 控制台打印，输出到文件
- 记录运行时信息，
- 异步模式，自动归档



#### 常见日志框架

- JUL
- JCL
- Jboss-logging
- logback
- log4j
- log4j2
- slf4j

| 日志接口/抽象层                                              | 日志实现层                                     |
| ------------------------------------------------------------ | ---------------------------------------------- |
| ~~JCL（Jakarta Commons Logging）~~    SLF4J(Simple Logging Facade for Java) ~~jboss-loggin~~ | Log4j  JUL (Java.util.loggin)  Log4j2  Logback |

选择日志接口抽象层：SLF4J

日志实现：Logback



SpringBoot:地层是Spring框架，Spring框架默认是使用的JCL；

==**SpringBoot选用的是SLF4J和logback;**==



## 2、SLF4j使用

### 1、如何在系统中使用SLF4j

[slf4j官网]: http://www.slf4j.org/

调用日志抽象层方法。

给系统导入slf4j的jar和logback的实现jar；

```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class HelloWorld {
  public static void main(String[] args) {
    Logger logger = LoggerFactory.getLogger(HelloWorld.class);
    logger.info("Hello World");
  }
}
```



<img src="images\concrete-bindings.png" style="zoom:200%;" />

每一个日志的实现框架都有自己的配置文件。使用slf4j以后，**配置文件还是要做成日志实现框架的自己本身的配置文件**。



### 2、遗留问题

多框架项目中，不同框架使用了不同的日志文件系统；

统一日志记录，即使是不同的框架不同的日志系统，统一使用slf4j+logback进行日志管理。

![统一slf4j](images\legacy.png)

#### 统一slf4j日志

- 将系统其他日志框架用响应的  **-over-slf4j.jar 或  *-to-slf4j.jar替换掉；
- 再实现slf4j



```xml
	<parent>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starters</artifactId>
		<version>${revision}</version>
	</parent>
	<artifactId>spring-boot-starter</artifactId>
	<name>Spring Boot Starter</name>
	<description>Core starter, including auto-configuration support, logging and YAML</description>

```

```xml
	<parent>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starters</artifactId>
		<version>${revision}</version>
	</parent>
	<artifactId>spring-boot-starter-logging</artifactId>
	<name>Spring Boot Logging Starter</name>
	<description>Starter for logging using Logback. Default logging starter</description>
```

![image-20200221164853274](images\spring-boot-3-001.png)

#### 总结

1. SpringBoot底层使用的是slf4j+logback的方式进行日志记录
2. SpringBoot把其他的日志都替换成了slf4j，**将其他日志jar包排除（maven中使用 exclude标签），并导入相应的替换jar包**。

```xml
		<plugin>
				<groupId>org.apache.maven.plugins</groupId>
				<artifactId>maven-enforcer-plugin</artifactId>
				<executions>
					<execution>
						<id>enforce-rules</id>
						<goals>
							<goal>enforce</goal>
						</goals>
						<configuration>
							<rules>
								<bannedDependencies>
									<excludes>
										<exclude>commons-logging:*:*</exclude>
										<exclude>org.hibernate:hibernate-validator:*</exclude>
									</excludes>
									<searchTransitive>true</searchTransitive>
								</bannedDependencies>
								<dependencyConvergence />
							</rules>
							<fail>true</fail>
						</configuration>
					</execution>
				</executions>
			</plugin>
```



## 3、日志使用

### 1.默认日志级别

trace < debug < info < warn < error 

SpringBoot日志默认级别称之为root级别 (也就是info级别)

```java
@RunWith(SpringRunner.class)
@SpringBootTestpublic class DemoApplicationTest { 
    Logger logger = LoggerFactory.getLogger(getClass());  
    
    @Test   
    public void testPerson() {      
        //日志的级别 trace < debug < info < warn < error      
        //可以调整输出的日志文件级别      
        logger.trace("跟踪轨迹");      
        logger.debug("debug日志");    
        logger.info("这是Info");     
        logger.warn("warn..");      
        logger.error("ee");  
    }
}
```



![image-20200221194449632](images\loginfo.png)

### 2.调整日志级别

```properties
logging.level.com.demo=trace  ##调整com.demo包的日志级别
```

![image-20200221194726447](images\image-20200221194726447.png)

### 3.文件输出

下面的两者选择一个即可

- **logging.file**=xx.log   ##不指定路径，当前项目下生成log.log日志文件；也可以指定绝对路径级文件名，生成在指定文件下
- **logging.path**=**/ 指定目录



```properties
#在控制台输出的日志格式
logging.pattern.console=

#指定文件中输出的日志个格式
logging.pattern.file=
```

### 4.源码分析

![image-20200222074142120](images\spring-boot-3-002.png)

![image-20200222074340498](images\image-20200222074340498.png)



base.xml

```xml
<?xml version="1.0" encoding="UTF-8"?>

<!--
Base logback configuration provided for compatibility with Spring Boot 1.1
-->

<included>
	<include resource="org/springframework/boot/logging/logback/defaults.xml" />
	<property name="LOG_FILE" value="${LOG_FILE:-${LOG_PATH:-${LOG_TEMP:-${java.io.tmpdir:-/tmp}}}/spring.log}"/>
	<include resource="org/springframework/boot/logging/logback/console-appender.xml" />
	<include resource="org/springframework/boot/logging/logback/file-appender.xml" />
	<root level="INFO">
		<appender-ref ref="CONSOLE" />
		<appender-ref ref="FILE" />
	</root>
</included>

```



defaults.xml

```xml
<?xml version="1.0" encoding="UTF-8"?>

<!--
Default logback configuration provided for import
-->

<included>
	<conversionRule conversionWord="clr" converterClass="org.springframework.boot.logging.logback.ColorConverter" />
	<conversionRule conversionWord="wex" converterClass="org.springframework.boot.logging.logback.WhitespaceThrowableProxyConverter" />
	<conversionRule conversionWord="wEx" converterClass="org.springframework.boot.logging.logback.ExtendedWhitespaceThrowableProxyConverter" />
	<property name="CONSOLE_LOG_PATTERN" value="${CONSOLE_LOG_PATTERN:-%clr(%d{${LOG_DATEFORMAT_PATTERN:-yyyy-MM-dd HH:mm:ss.SSS}}){faint} %clr(${LOG_LEVEL_PATTERN:-%5p}) %clr(${PID:- }){magenta} %clr(---){faint} %clr([%15.15t]){faint} %clr(%-40.40logger{39}){cyan} %clr(:){faint} %m%n${LOG_EXCEPTION_CONVERSION_WORD:-%wEx}}"/>
	<property name="FILE_LOG_PATTERN" value="${FILE_LOG_PATTERN:-%d{${LOG_DATEFORMAT_PATTERN:-yyyy-MM-dd HH:mm:ss.SSS}} ${LOG_LEVEL_PATTERN:-%5p} ${PID:- } --- [%t] %-40.40logger{39} : %m%n${LOG_EXCEPTION_CONVERSION_WORD:-%wEx}}"/>

	<logger name="org.apache.catalina.startup.DigesterFactory" level="ERROR"/>
	<logger name="org.apache.catalina.util.LifecycleBase" level="ERROR"/>
	<logger name="org.apache.coyote.http11.Http11NioProtocol" level="WARN"/>
	<logger name="org.apache.sshd.common.util.SecurityUtils" level="WARN"/>
	<logger name="org.apache.tomcat.util.net.NioSelectorPool" level="WARN"/>
	<logger name="org.eclipse.jetty.util.component.AbstractLifeCycle" level="ERROR"/>
	<logger name="org.hibernate.validator.internal.util.Version" level="WARN"/>
</included>

```



console-appender.xml

```xml
<?xml version="1.0" encoding="UTF-8"?>

<!--
Console appender logback configuration provided for import, equivalent to the programmatic
initialization performed by Boot
-->

<included>
	<appender name="CONSOLE" class="ch.qos.logback.core.ConsoleAppender">
		<encoder>
			<pattern>${CONSOLE_LOG_PATTERN}</pattern>
		</encoder>
	</appender>
</included>

```



file-appender.xml

```xml
<?xml version="1.0" encoding="UTF-8"?>

<!--
File appender logback configuration provided for import, equivalent to the programmatic
initialization performed by Boot
-->

<included>
	<appender name="FILE"
		class="ch.qos.logback.core.rolling.RollingFileAppender">
		<encoder>
			<pattern>${FILE_LOG_PATTERN}</pattern>
		</encoder>
		<file>${LOG_FILE}</file>
		<rollingPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedRollingPolicy">
			<cleanHistoryOnStart>${LOG_FILE_CLEAN_HISTORY_ON_START:-false}</cleanHistoryOnStart>
			<fileNamePattern>${ROLLING_FILE_NAME_PATTERN:-${LOG_FILE}.%d{yyyy-MM-dd}.%i.gz}</fileNamePattern>
			<maxFileSize>${LOG_FILE_MAX_SIZE:-10MB}</maxFileSize>
			<maxHistory>${LOG_FILE_MAX_HISTORY:-7}</maxHistory>
			<totalSizeCap>${LOG_FILE_TOTAL_SIZE_CAP:-0}</totalSizeCap>
		</rollingPolicy>
	</appender>
</included>

```





系统配置的日志环境变量加载到LoggingSystemProperties.java中

```java
package org.springframework.boot.logging;

import org.springframework.boot.system.ApplicationPid;
import org.springframework.core.env.ConfigurableEnvironment;
import org.springframework.core.env.Environment;
import org.springframework.core.env.PropertyResolver;
import org.springframework.core.env.PropertySourcesPropertyResolver;
import org.springframework.util.Assert;

/**
 * Utility to set system properties that can later be used by log configuration files.
 *
 * @author Andy Wilkinson
 * @author Phillip Webb
 * @author Madhura Bhave
 * @author Vedran Pavic
 * @author Robert Thornton
 * @author Eddú Meléndez
 * @since 2.0.0
 */
public class LoggingSystemProperties {
    
	/**
	 * The name of the System property that contains the process ID.
	 */
	public static final String PID_KEY = "PID";

	/**
	 * The name of the System property that contains the exception conversion word.
	 */
	public static final String EXCEPTION_CONVERSION_WORD = "LOG_EXCEPTION_CONVERSION_WORD";

	/**
	 * The name of the System property that contains the log file.
	 */
	public static final String LOG_FILE = "LOG_FILE";

	/**
	 * The name of the System property that contains the log path.
	 */
	public static final String LOG_PATH = "LOG_PATH";

	/**
	 * The name of the System property that contains the console log pattern.
	 */
	public static final String CONSOLE_LOG_PATTERN = "CONSOLE_LOG_PATTERN";

	/**
	 * The name of the System property that contains the clean history on start flag.
	 */
	public static final String FILE_CLEAN_HISTORY_ON_START = "LOG_FILE_CLEAN_HISTORY_ON_START";

	/**
	 * The name of the System property that contains the file log pattern.
	 */
	public static final String FILE_LOG_PATTERN = "FILE_LOG_PATTERN";

	/**
	 * The name of the System property that contains the file log max history.
	 */
	public static final String FILE_MAX_HISTORY = "LOG_FILE_MAX_HISTORY";

	/**
	 * The name of the System property that contains the file log max size.
	 */
	public static final String FILE_MAX_SIZE = "LOG_FILE_MAX_SIZE";

	/**
	 * The name of the System property that contains the file total size cap.
	 */
	public static final String FILE_TOTAL_SIZE_CAP = "LOG_FILE_TOTAL_SIZE_CAP";

	/**
	 * The name of the System property that contains the log level pattern.
	 */
	public static final String LOG_LEVEL_PATTERN = "LOG_LEVEL_PATTERN";

	/**
	 * The name of the System property that contains the log date-format pattern.
	 */
	public static final String LOG_DATEFORMAT_PATTERN = "LOG_DATEFORMAT_PATTERN";

	/**
	 * The name of the System property that contains the rolled-over log file name
	 * pattern.
	 */
	public static final String ROLLING_FILE_NAME_PATTERN = "ROLLING_FILE_NAME_PATTERN";

	private final Environment environment;

	/**
	 * Create a new {@link LoggingSystemProperties} instance.
	 * @param environment the source environment
	 */
	public LoggingSystemProperties(Environment environment) {
		Assert.notNull(environment, "Environment must not be null");
		this.environment = environment;
	}

	public void apply() {
		apply(null);
	}

	public void apply(LogFile logFile) {
		PropertyResolver resolver = getPropertyResolver();
		setSystemProperty(resolver, EXCEPTION_CONVERSION_WORD, "exception-conversion-word");
		setSystemProperty(PID_KEY, new ApplicationPid().toString());
		setSystemProperty(resolver, CONSOLE_LOG_PATTERN, "pattern.console");
		setSystemProperty(resolver, FILE_LOG_PATTERN, "pattern.file");
		setSystemProperty(resolver, FILE_CLEAN_HISTORY_ON_START, "file.clean-history-on-start");
		setSystemProperty(resolver, FILE_MAX_HISTORY, "file.max-history");
		setSystemProperty(resolver, FILE_MAX_SIZE, "file.max-size");
		setSystemProperty(resolver, FILE_TOTAL_SIZE_CAP, "file.total-size-cap");
		setSystemProperty(resolver, LOG_LEVEL_PATTERN, "pattern.level");
		setSystemProperty(resolver, LOG_DATEFORMAT_PATTERN, "pattern.dateformat");
		setSystemProperty(resolver, ROLLING_FILE_NAME_PATTERN, "pattern.rolling-file-name");
		if (logFile != null) {
			logFile.applyToSystemProperties();
		}
	}

	private PropertyResolver getPropertyResolver() {
		if (this.environment instanceof ConfigurableEnvironment) {
			PropertySourcesPropertyResolver resolver = new PropertySourcesPropertyResolver(
					((ConfigurableEnvironment) this.environment).getPropertySources());
			resolver.setIgnoreUnresolvableNestedPlaceholders(true);
			return resolver;
		}
		return this.environment;
	}

	private void setSystemProperty(PropertyResolver resolver, String systemPropertyName, String propertyName) {
		setSystemProperty(systemPropertyName, resolver.getProperty("logging." + propertyName));
	}

	private void setSystemProperty(String name, String value) {
		if (System.getProperty(name) == null && value != null) {
			System.setProperty(name, value);
		}
	}
}
```



在LoggingApplicationListener.java中 通过 LoggingSystemProperties读入配置属性

```java
package org.springframework.boot.context.logging;

public class LoggingApplicationListener implements GenericApplicationListener {
    
    protected void initialize(ConfigurableEnvironment environment, ClassLoader classLoader) {
		new LoggingSystemProperties(environment).apply();
		this.logFile = LogFile.get(environment);
		if (this.logFile != null) {
			this.logFile.applyToSystemProperties();
		}
		this.loggerGroups = new LoggerGroups(DEFAULT_GROUP_LOGGERS);
		initializeEarlyLoggingLevel(environment);
		initializeSystem(environment, this.loggingSystem, this.logFile);
		initializeFinalLoggingLevels(environment, this.loggingSystem);
		registerShutdownHookIfNecessary(environment, this.loggingSystem);
	}
    
}
```



### 5、指定日志配置

#### 4.4.6. Custom Log Configuration

The various logging systems can be activated by including the  appropriate libraries on the classpath and can be further customized by  providing a suitable configuration file in the root of the classpath or  in a location specified by the following Spring `Environment` property: `logging.config`.

You can force Spring Boot to use a particular logging system by using the **org.springframework.boot.logging.LoggingSystem** system property. The value should be the fully qualified class name of a **`LoggingSystem`** implementation. You can also disable Spring Boot’s logging configuration entirely by using a value of `none`.

> **Since logging is initialized before the ApplicationContext is created, it is not possible to control logging from @PropertySources in Spring @Configuration files. The only way to change the logging system or disable it entirely is via System properties.** 

>  **由于日志记录是在创建ApplicationContext之前初始化的，因此无法从Spring @Configuration文件中的@PropertySources控制日志记录。 更改日志记录系统或完全禁用它的唯一方法是通过系统属性。** 

Depending on your logging system, the following files are loaded:

| Logging System          | Customization                                                |
| ----------------------- | ------------------------------------------------------------ |
| Logback                 | `logback-spring.xml`, `logback-spring.groovy`, `logback.xml`, or `logback.groovy` |
| Log4j2                  | `log4j2-spring.xml` or `log4j2.xml`                          |
| JDK (Java Util Logging) | `logging.properties`                                         |



> **When possible, we recommend that you use the -spring variants for your logging configuration (for example, logback-spring.xml rather than logback.xml). If you use standard configuration locations, Spring cannot completely control log initialization.** 

>  **如果可能，我们建议您在日志配置中使用-spring变体（例如，logback-spring.xml而不是logback.xml）。 如果使用标准配置位置，Spring将无法完全控制日志初始化。** 

logback.xml 直接被日志框架识别；

logback-spring.xml 日志框架不会加载该日志的配置项，而是由SpringBoot解析日志配置，可以使用SpringBoot的高级Profile功能

```xml
<springProfile name="staging"><!--staging某个环境   dev or produ   是通过application.properties中 spring.profiles.active=dev  决定的-->
    <!-- configuration to be enabled when the "staging" profile is active -->
    <!--可以指定某段配置只在某个环境下生效-->
</springProfile>
```

