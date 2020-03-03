

# SpringBoot Web开发



## 1、SpringBoot对静态资源的映射规则

### 

```java
package org.springframework.boot.autoconfigure.web.servlet;

@Configuration(proxyBeanMethods = false)
@ConditionalOnWebApplication(type = Type.SERVLET)
@ConditionalOnClass({ Servlet.class, DispatcherServlet.class, WebMvcConfigurer.class })
@ConditionalOnMissingBean(WebMvcConfigurationSupport.class)
@AutoConfigureOrder(Ordered.HIGHEST_PRECEDENCE + 10)
@AutoConfigureAfter({ DispatcherServletAutoConfiguration.class, TaskExecutionAutoConfiguration.class,
		ValidationAutoConfiguration.class })
public class WebMvcAutoConfiguration {
    
    /**配置首页映射
    mvcProperties.getStaticPathPattern="/**"
   	getWelcomePage 最终方法体是  index.html
   		private Resource getIndexHtml(String location) {
			return this.resourceLoader.getResource(location + "index.html");
		}
    */
    @Bean
    public WelcomePageHandlerMapping welcomePageHandlerMapping(ApplicationContext applicationContext,
                                                               FormattingConversionService mvcConversionService, ResourceUrlProvider mvcResourceUrlProvider) {
        WelcomePageHandlerMapping welcomePageHandlerMapping = new WelcomePageHandlerMapping(
            new TemplateAvailabilityProviders(applicationContext), applicationContext, getWelcomePage(),
            this.mvcProperties.getStaticPathPattern());
        welcomePageHandlerMapping.setInterceptors(getInterceptors(mvcConversionService, mvcResourceUrlProvider));
        return welcomePageHandlerMapping;
    }

    //添加资源映射
    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        if (!this.resourceProperties.isAddMappings()) {
            logger.debug("Default resource handling disabled");
            return;
        }
        Duration cachePeriod = this.resourceProperties.getCache().getPeriod();
        CacheControl cacheControl = this.resourceProperties.getCache().getCachecontrol().toHttpCacheControl();
        // /webjars/**
        if (!registry.hasMappingForPattern("/webjars/**")) {
            customizeResourceHandlerRegistration(registry.addResourceHandler("/webjars/**")
                                                 .addResourceLocations("classpath:/META-INF/resources/webjars/")
                                                 .setCachePeriod(getSeconds(cachePeriod)).setCacheControl(cacheControl));
        }
        
        /**	
        * 	1.mvcProperties.getStaticPathPattern()是由WebMvcProperties中staticPathPattern= "/**";
        *	2.resourceProperties.getStaticLocations()是由ResourceProperties  
  		*	String[] CLASSPATH_RESOURCE_LOCATIONS = { "classpath:/META-INF/resources/","classpath:/resources/", "classpath:/static/", 		  *	  "classpath:/public/" };
        */
			
        String staticPathPattern = this.mvcProperties.getStaticPathPattern();
        if (!registry.hasMappingForPattern(staticPathPattern)) {
            customizeResourceHandlerRegistration(registry.addResourceHandler(staticPathPattern)
                                                 	.addResourceLocations(getResourceLocations(
                                                        this.resourceProperties.getStaticLocations())
                                                                         )
                                                 .setCachePeriod(getSeconds(cachePeriod)).setCacheControl(cacheControl));
        }
    }
    
}
```

### 1、  /webjars/**

所有/webjars/**都到classpath:/META-INF/resources/webjars/找资源

所谓webjars就是以jar的方式引入静态资源；

[webjars官网]: https://www.webjars.org/

- 将静态资源版本化，更利于升级和维护。
- 剥离静态资源，提高编译速度和打包效率。
- 实现资源共享，有利于统一前端开发。

![image-20200222101637039](images\image-20200222101637039.png)

- 引入

  ```xml
  <dependency>
  			<groupId>org.webjars.bower</groupId>
  			<artifactId>jquery</artifactId>
  			<version>3.4.1</version>
  		</dependency>
  ```

  ![image-20200222102231380](images\image-20200222102231380.png)



resources.webjars.jquery.xx 正好对应classpath:/META-INF/resources/webjars/映射

访问路径为：http://localhost:8080/webjars/jquery/3.4.1/src/jquery.js可以访问到jquery.js文件

### 2、/**

 /** 访问当前项目的任何资源，（静态资源文件夹有）

```
"classpath:/META-INF/resources/", 
"classpath:/resources/",
"classpath:/static/", 
"classpath:/public/" 
"/" :当前项目跟路径
```

#### 注意:

- classpath指的是项目中java文件夹下或者resources文件夹下

  ![image-20200222104959558](images\image-20200222104959558.png)

  所以在创建 /META-INF/resources/ 或者 /resources/  、/static/、/public/

![image-20200222105230750](images\image-20200222105230750.png)

- 在访问的时候要加载到指定的/下，不要加配置资源默认扫描的前缀信息

  即不要加上 /META-INF/resources/ 或者 /resources/  、/static/、/public/ 这些信息

  http://localhost:8080/js/assert/alert.js



### 3、首页

所有静态资源文件夹下的index.html

### 4、自定义静态资源文件夹

```java
@ConfigurationProperties(prefix = "spring.resources", ignoreUnknownFields = false)
public class ResourceProperties {
 	private static final String[] CLASSPATH_RESOURCE_LOCATIONS = { "classpath:/META-INF/resources/",
			"classpath:/resources/", "classpath:/static/", "classpath:/public/" };

	/**
	 * Locations of static resources. Defaults to classpath:[/META-INF/resources/,
	 * /resources/, /static/, /public/].
	 */
	private String[] staticLocations = CLASSPATH_RESOURCE_LOCATIONS;
    
}
```

application.properites中自定义

```xml
spring.resources.static-locations=classpath:/resource/,classpath:/public/
```

自定义完成后，默认的静态资源文件夹配置就被替换了（不在生效）。



## 2、模板引擎

JSP、Velocity、Freemarker、Thymeleaf；

![image-20200222152608768](images\image-20200222152608768.png)

### SpringBoot推荐的Thymeleaf:

[Thymele官网]: https://www.thymeleaf.org/	"Thymeleaf"

//TODO 下载thymeleaf.pdfhttps://www.thymeleaf.org/documentation.html

#### 1、引入Thymeleaf

```xml
<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-thymeleaf</artifactId>
		</dependency>
```



### 2、Thymeleaf语法

```java
package org.springframework.boot.autoconfigure.thymeleaf;


@ConfigurationProperties(prefix = "spring.thymeleaf")
public class ThymeleafProperties {

	private static final Charset DEFAULT_ENCODING = StandardCharsets.UTF_8;

    //扫描生效文件位置
	public static final String DEFAULT_PREFIX = "classpath:/templates/";

    //后缀
	public static final String DEFAULT_SUFFIX = ".html";
    
}
```

只要把 .html页面放在 classpath:/templates/ ，thymeleaf就能自动渲染；

#### 1、在.html页面导入Thymeleaf的名称空间

#### 2、thymeleaf语法规则

- th:任意html属性；来替换原生属性的值
- 表达式



## 3、Spring MVC



[Spring Boot 自动配置SpringMVC]: https://docs.spring.io/spring-boot/docs/2.3.0.M2/reference/htmlsingle/#boot-features-spring-mvc

### Spring MVC Auto-configuration

> Spring Boot provides auto-configuration for Spring MVC that works well with most applications.
>
> The auto-configuration adds the following features on top of Spring’s defaults:
>
> - Inclusion of ==`ContentNegotiatingViewResolver`== and ==`BeanNameViewResolver`== beans.
>
>   - 自动配置了ViewResolver（视图解析器：根据方法的返回值得到视图对象（View），视图对象决定如何渲染）
>
>     ```java
>     package org.springframework.boot.autoconfigure.web.servlet;
>     
>     import org.springframework.web.servlet.view.BeanNameViewResolver;
>     import org.springframework.web.servlet.view.ContentNegotiatingViewResolver;
>     import org.springframework.web.servlet.view.InternalResourceViewResolver;
>     
>     public class WebMvcAutoConfiguration {
>         
>     		@Bean
>     		@ConditionalOnBean(ViewResolver.class)
>     		@ConditionalOnMissingBean(name = "viewResolver", value = ContentNegotiatingViewResolver.class)
>     		public ContentNegotiatingViewResolver viewResolver(BeanFactory beanFactory) {
>     			ContentNegotiatingViewResolver resolver = new ContentNegotiatingViewResolver();
>     			resolver.setContentNegotiationManager(beanFactory.getBean(ContentNegotiationManager.class));
>     			// ContentNegotiatingViewResolver uses all the other view resolvers to locate
>     			// a view so it should have a high precedence
>     			resolver.setOrder(Ordered.HIGHEST_PRECEDENCE);
>     			return resolver;
>     		}
>         
>     }
>     ```
>
>     ContentNegotiatingViewResolver :组合所有的视图解析器，我们自定义一个视图解析器 implements ViewResolver接口，通过@Bean添加到容器，ContentNegotiatingViewResolver 自动扫描添加；
>
> - Support for serving static resources, including support for WebJars 
>
>   - 支持提供静态资源，包括对WebJars的支持 
>
> - Automatic registration of `Converter`, `GenericConverter`, and `Formatter` beans.
>
>   - ​	自动注册了
>     - `Converter`（转换器==请求当中类型转换   包括 json => Objct ，string to Boolean.）
>     - `GenericConverter`
>     - ``Formatter` 格式化 2017-12-25 to Date
>
> - Support for `HttpMessageConverters`
>
>   - `HttpMessageConverters`SpringMVC用来转换Http请求和响应的；User--Json
>
> - Automatic registration of `MessageCodesResolver`
>
> - Static `index.html` support.
>
> - Custom `Favicon` support 
>
> - Automatic use of a `ConfigurableWebBindingInitializer` bean 



### 如何修改SpringBoot的默认配置

模式：

1. SpringBoot在自动配置很多组件的时候，先看容器中有没有用户配置的相关组件（@Bean、@Componet）,如果有就使用用户的配置，如果没有系统自动配置；如果有些组件可以有多个，则将用户配置和系统默认的组合起来。
2. 在SpringBoot中会有非常多的xxxConfigurer帮助我们进行扩展配置。



### 扩展SpringMVC

> **If you want to keep those Spring Boot MVC customizations and make more [MVC customizations](https://docs.spring.io/spring/docs/5.2.3.RELEASE/spring-framework-reference/web.html#mvc) (interceptors, formatters, view controllers, and other features), you can add your own `@Configuration` class of type `WebMvcConfigurer` but without `@EnableWebMvc`.**
>
> -  **如果要保留这些Spring Boot MVC定制并进行更多的MVC定制（拦截器，格式化程序，视图控制器和其他功能），则可以添加自己的类型为WebMvcConfigurer的@Configuration类，但不添加@EnableWebMvc。** 

编写一个配置类（@Configuration  ），是WebMvcConfigurer类型~~(WebMvcConfigurerAdapter过时不能用)~~，但不添加@EnableWebMvc。

原理：

1. ```java
   WebMvcAutoConfiguration
   ```

2. ```java
   @Configuration(proxyBeanMethods = false)
   @Import(EnableWebMvcConfiguration.class)
   @EnableConfigurationProperties({ WebMvcProperties.class, ResourceProperties.class })
   @Order(0)
   public static class WebMvcAutoConfigurationAdapter implements WebMvcConfigurer {
       
   }
   ```

   @Import(EnableWebMvcConfiguration.class)

> **If you want to provide custom instances of `RequestMappingHandlerMapping`, `RequestMappingHandlerAdapter`, or `ExceptionHandlerExceptionResolver`, and still keep the Spring Boot MVC customizations, you can declare a bean of type `WebMvcRegistrations` and use it to provide custom instances of those components.**
>
> - **如果要提供RequestMappingHandlerMapping，RequestMappingHandlerAdapter或ExceptionHandlerExceptionResolver的自定义实例，并且仍然保留Spring Boot MVC自定义，则可以声明WebMvcRegistrations类型的bean，并使用它提供那些组件的自定义实例。** 



> **If you want to take complete control of Spring MVC, you can add your own `@Configuration` annotated with `@EnableWebMvc`, or alternatively add your own `@Configuration`-annotated `DelegatingWebMvcConfiguration` as described in the Javadoc of `@EnableWebMvc`.**
>
> -  **如果要完全控制Spring MVC，则可以添加用@EnableWebMvc注释的自己的@Configuration，或者按照@EnableWebMvc的Javadoc中的说明添加自己的@Configuration注释的DelegatingWebMvcConfiguration。** 

全面接管SpringMVC；在自己的@Configuration上加@EnableWebMvc；SpringMVC的自动配置全部失效。

### 

## 4、错误处理机制

错误处理的自动配置：

```java
package org.springframework.boot.autoconfigure.web.servlet.error;
/** * {@link EnableAutoConfiguration Auto-configuration} to render errors via an MVC error * controller. * * @author Dave Syer * @author Andy Wilkinson * @author Stephane Nicoll * @author Brian Clozel * @since 1.0.0 */
@Configuration(proxyBeanMethods = false)
@ConditionalOnWebApplication(type = Type.SERVLET)
@ConditionalOnClass({ Servlet.class, DispatcherServlet.class })
// Load before the main WebMvcAutoConfiguration so that the error View is available
@AutoConfigureBefore(WebMvcAutoConfiguration.class)
@EnableConfigurationProperties({ ServerProperties.class, ResourceProperties.class, WebMvcProperties.class })
public class ErrorMvcAutoConfiguration {
    private final ServerProperties serverProperties;

}
```





## 5、配置嵌入式Servlet容器

```java
package org.springframework.boot.autoconfigure.web;
@ConfigurationProperties(prefix = "server", ignoreUnknownFields = true)
public class ServerProperties {

}
```



## 6、配置嵌入式Servlet容器

#### 1、注册servlet、Filter、Listener

- ServletRegistrationBean
- FilterRegistrationBean
- ServletListenerRegistrationBean