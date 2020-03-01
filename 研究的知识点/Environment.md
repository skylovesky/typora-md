# Environment

![image-20200223171325748](..\images\image-20200223171325748.png)



>  `ConfigurableXXX`成了Spring的一种命名规范，或者说是一种设计模式。它表示课配置的，所以都会提供大量的set方法 
>
>  Spring很多接口都是读写分离的，最顶层接口一般都只会提供只读方法，这是Spring框架设计的一般规律之一 

### 属性管理 API

### 概述

- `PropertyResolver`  此接口用于在**底层源**之上解析**一系列**的属性值：例如properties文件,yaml文件,甚至是一些nosql（因为nosql也是k-v形式） 

  - `Environment` 个接口代表了当前应用正在运行的环境，为应用的两个重要方面建立抽象模型 【`profiles`】和【`properties`】。关于属性访问的方法通过父接口`PropertyResolver`暴露给客户端使用，本接口主要是扩展出访问【`profiles`】相关的接口。

     `profiles`：配置。它代表应用在一启动时注册到context中bean definitions的命名的逻辑分组 

     `properties`：属性。几乎在所有应用中都扮演着重要角色，他可能源自多种源头。例如属性文件，JVM系统属性，系统环境变量，JNDI，servlet上下文参数，	Map等等，**Environment对象和其相关的对象**一起提供给用户一个方便用来配置和解析属性的服务。 

     **我们可以通过实现接口`EnvironmentAware`或者直接`@Autowired`可以很方便的得到当前应用的环境：`Environment`** 

  - `ConfigurablePropertyResolver` 它是一个可配置的处理器。这个方法不仅有父接口所有功能，还扩展定义类型转换、属性校验、前缀、后缀、`分隔符`等一些列的功能，这个在具体实现类里有所体现

    - `AbstractPropertyResolver ` 是对 `ConfigurablePropertyResolver`的一个抽象实现，实现了了所有的接口方法，并且只提供一个抽象方法给子类去实现 

  

#### Environment源码

```java
/*
	环境，本身是一个 PropertyResolver，但是提供了 Profile 特性，
	即可以根据环境得到相应数据（即激活不同的 Profile，可以得到不同的属性数据，
	比如用于多环境场景的配置（正式机、测试机、开发机 DataSource 配置）
 */

package org.springframework.core.env;

/**
 */
public interface Environment extends PropertyResolver {

	String[] getActiveProfiles();

	String[] getDefaultProfiles();

	@Deprecated
	boolean acceptsProfiles(String... profiles);

	boolean acceptsProfiles(Profiles profiles);
}

```



#### PropertyResolver源码

```java
/**
此接口用于在底层源之上解析一系列的属性值：例如properties文件,yaml文件,甚至是一些nosql（因为nosql也是k-v形式）。
*/
package org.springframework.core.env;

import org.springframework.lang.Nullable;

public interface PropertyResolver {

    // 查看规定指定的key是否有对应的value   注意：若对应值是null的话 也是返回false
	boolean containsProperty(String key);

    // 如果没有则返回null
	@Nullable
	String getProperty(String key);

    // 如果没有则返回defaultValue
	String getProperty(String key, String defaultValue);

    // 返回指定key对应的value，会解析成指定类型。如果没有对应值则返回null（而不是抛错~）
	@Nullable
	<T> T getProperty(String key, Class<T> targetType);

	<T> T getProperty(String key, Class<T> targetType, T defaultValue);

    // 若不存在就不是返回null了  而是抛出异常~  所以不用担心返回值是null
	String getRequiredProperty(String key) throws IllegalStateException;

	<T> T getRequiredProperty(String key, Class<T> targetType) throws IllegalStateException;
	
    // 解析${...}这种类型的占位符，把他们替换为使用getProperty方法返回的结果，解析不了并且没有默认值的占位符会被忽略（原样输出）
	String resolvePlaceholders(String text);

	// 解析不了就抛出异常~
	String resolveRequiredPlaceholders(String text) throws IllegalArgumentException;

}

```





```java
/*
 * 
 */
```



####  Profiles 源码

```java

package org.springframework.core.env;

import java.util.function.Predicate;

@FunctionalInterface
public interface Profiles {

	boolean matches(Predicate<String> activeProfiles);

	static Profiles of(String... profiles) {
		return ProfilesParser.parse(profiles);
	}

}

```

#### Profile源码

```java
/*
 
 */

package org.springframework.context.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import org.springframework.core.env.AbstractEnvironment;
import org.springframework.core.env.ConfigurableEnvironment;
import org.springframework.core.env.Profiles;

/**
 * 
 */
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Conditional(ProfileCondition.class)
public @interface Profile {

	String[] value();

}

```

