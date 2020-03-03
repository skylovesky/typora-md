# Spring Boot（一）

##### 启动

```java
@SpringBootApplication
```



##### 组件绑定配置文件中的属性

###### @ConfigurationProperties 

 **默认是从全局配置文件中获取** 

```java
@Component
@ConfigurationProperties(prefix = "person")//  从全局配置文件中获取  绑定配置文件中的属性 prefix 指定前缀 而 要想绑定配置文件的属性,该类必须是Spring容器中的组件,所以加上 @Component
```

###### @Value("")    

```java
@Component
public class PersonVO {   
    
    @Value("${person.name}")//在配置文件获取
    private String name;
    
	@Value("#{11*2}")//计算
	private Integer age;

	@Value("true")//直接赋值
	private Boolean boss;
}
```

###### @Value和@ConfigurationProperties比较

|                  | @ConfigurationProperties                     | @Value         |
| :--------------- | -------------------------------------------- | -------------- |
| 功能             | 批量注入配置文件中属性                       | 一个一个绑定   |
| 支持松散语法绑定 | 支持: lastName   last-name                   | 不支持         |
| SpEL表达式       | 不支持                                       | 支持  #{11*33} |
| JSR303数据校验   | 支持  @Validated(类上)  @Email（属性上） ... | 不支持         |
| 复杂类型封装     | 支持                                         | 不支持         |



###### @PropertySource 加载指定配置文件

```java
@PropertySource(value = {"classpath:person.properties"})//数组
```



###### @ImportResource导入Spring配置文件   不推荐

```java
@ImportResource(locations = {"classpath:beans.xml"})
```

自定义一个Config类来实现spring组件注入

```java
@Configuration
public class MyConfig {
	//将方法的返回值添加到容器中，容器中这个组件的默认id就是这个方法名 （helloService001）
	@Bean
    public HelloService helloService001(){
        return new HelloService();
    }
}
```

###### 配置文件占位符${}



##### 单元测试

```java
@RunWith(SpringRunner.class)//启动器为SpringRunner而不是Juntil
@SpringBootTest//springBoot的单元测试
public class DemoApplicationTest {
	
    @Autowired//注意这里可以直接注入的方式测试
	private PersonVO personVO;
    
    @Test
	public void testPerson(){
		System.out.printf(String.valueOf(personVO));
	}
}

```



#### 5.Profile

##### 1.多Profile文件

我们在主配置文件编写的时候，文件名可是是application-{profile}.properties/yml

默认使用application.properties/yml的配置

![image-20200220153508446](images\spring-boot-0-003.png)





##### 2.yml支持多文档块方式

```yaml
spring:
	profiles:
		active: dev
server:
	port: 8080
---#文档块
server:
	port: 8081
spring:
	profiles: dev
---#文档块
server:
	port: 8082
spring:
	profiles: product
```

##### 3.激活指定profile

- 在配置文件中指定

  - ```properties
    spring.profiles.active=dev
    ```

  - ```yaml
    spring: 
    	profiles:
        	active: dev
    ```

- 命令行模式：

  - --spring.profiles.active=dev



#### 6.配置文件加载位置

spring boot 启动会扫描一下位置的application.properties/yml文件作为Spring boot 的默认配置文件

- file:./config/  		指的是当前项目根目录下的config文件夹
- file:./                       指的是当前项目的根目录
- classpath:/config/
- classpath:/

以上是按照优先级从高到低的顺序，所有位置的文件都会被加载，**高优先级配置内容会为主**，互补配置。

可以通过配置spring.config.location来改变默认配置，配合命令行使用。



#### 7.外部配置加载顺序

#### 8.自动配置原理

[配置文件可配置属性参照]: https://docs.spring.io/spring-boot/docs/2.3.0.M2/reference/htmlsingle/#appendix	"本地"



##### 自动配置原理：

- SpringBoot启动时加载主配置类（**@SpringBootApplication**），

  开启了自动配置功能（**@EnableAutoConfiguration**）

- **@EnableAutoConfiguration**利用**AutoConfigurationImportSelector**给容器中导入组件

  ```java
  @Target(ElementType.TYPE)
  @Retention(RetentionPolicy.RUNTIME)
  @Documented
  @Inherited
  @AutoConfigurationPackage
  @Import(AutoConfigurationImportSelector.class)
  public @interface EnableAutoConfiguration {
      String ENABLED_OVERRIDE_PROPERTY = "spring.boot.enableautoconfiguration";
      Class<?>[] exclude() default {};
    	String[] excludeName() default {};
  }
  ```

  **@Import(AutoConfigurationImportSelector.class)**

  ```java
  public class AutoConfigurationImportSelector implements 
      DeferredImportSelector, BeanClassLoaderAware,      
  ResourceLoaderAware, BeanFactoryAware, EnvironmentAware, Ordered {
      //...
      @Override
  	public String[] selectImports(AnnotationMetadata annotationMetadata) {
  		if (!isEnabled(annotationMetadata)) {
  			return NO_IMPORTS;
  		}
  		AutoConfigurationMetadata autoConfigurationMetadata = AutoConfigurationMetadataLoader
  				.loadMetadata(this.beanClassLoader);
  		AutoConfigurationEntry autoConfigurationEntry = getAutoConfigurationEntry(autoConfigurationMetadata,
  				annotationMetadata);
  		return StringUtils.toStringArray(autoConfigurationEntry.getConfigurations());
  	}
      
      protected AutoConfigurationEntry getAutoConfigurationEntry(AutoConfigurationMetadata autoConfigurationMetadata,
  			AnnotationMetadata annotationMetadata) {
  		if (!isEnabled(annotationMetadata)) {
  			return EMPTY_ENTRY;
  		}
  		AnnotationAttributes attributes = getAttributes(annotationMetadata);
  		List<String> configurations = getCandidateConfigurations(annotationMetadata, attributes);
  		configurations = removeDuplicates(configurations);
  		Set<String> exclusions = getExclusions(annotationMetadata, attributes);
  		checkExcludedClasses(configurations, exclusions);
  		configurations.removeAll(exclusions);
  		configurations = filter(configurations, autoConfigurationMetadata);
  		fireAutoConfigurationImportEvents(configurations, exclusions);
  		return new AutoConfigurationEntry(configurations, exclusions);
  	}
      
      protected Class<?> getSpringFactoriesLoaderFactoryClass() {
  		return EnableAutoConfiguration.class;
  	}
      //...
  }
  ```

- List<String> configurations = getCandidateConfigurations(annotationMetadata, attributes);

```java
protected List<String> getCandidateConfigurations(
    AnnotationMetadata metadata, AnnotationAttributes attributes) {
    List<String> configurations = SpringFactoriesLoader.loadFactoryNames(getSpringFactoriesLoaderFactoryClass(),         getBeanClassLoader());
    Assert.notEmpty(configurations, 
                    "No auto configuration classes found in META-INF/spring.factories. If you are using a custom packaging, make sure that file is correct."
                   );   
    return configurations;
}
```

- List<String> configurations = SpringFactoriesLoader.loadFactoryNames(getSpringFactoriesLoaderFactoryClass(),         getBeanClassLoader());

**public static final String FACTORIES_RESOURCE_LOCATION = "META-INF/spring.factories";**

```java
public final class SpringFactoriesLoader{
	public static final String FACTORIES_RESOURCE_LOCATION = "META-INF/spring.factories";
    public static List<String> loadFactoryNames(Class<?> factoryType, @Nullable ClassLoader classLoader) {
		String factoryTypeName = factoryType.getName();
		return loadSpringFactories(classLoader).getOrDefault(factoryTypeName, Collections.emptyList());
	}

	private static Map<String, List<String>> loadSpringFactories(@Nullable ClassLoader classLoader) {
		MultiValueMap<String, String> result = cache.get(classLoader);
		if (result != null) {
			return result;
		}

		try {
			Enumeration<URL> urls = (classLoader != null ?
					classLoader.getResources(FACTORIES_RESOURCE_LOCATION) :
					ClassLoader.getSystemResources(FACTORIES_RESOURCE_LOCATION));
			result = new LinkedMultiValueMap<>();
			while (urls.hasMoreElements()) {
				URL url = urls.nextElement();
				UrlResource resource = new UrlResource(url);
				Properties properties = PropertiesLoaderUtils.loadProperties(resource);
				for (Map.Entry<?, ?> entry : properties.entrySet()) {
					String factoryTypeName = ((String) entry.getKey()).trim();
					for (String factoryImplementationName : StringUtils.commaDelimitedListToStringArray((String) entry.getValue())) {
						result.add(factoryTypeName, factoryImplementationName.trim());
					}
				}
			}
			cache.put(classLoader, result);
			return result;
		}
		catch (IOException ex) {
			throw new IllegalArgumentException("Unable to load factories from location [" +
					FACTORIES_RESOURCE_LOCATION + "]", ex);
		}
	}
}
```



流程是：

- 启动@SpringBootApplication

- 在@SpringBootApplication中有@EnableAutoConfiguration

- 在@EnableAutoConfiguration中有@Import(AutoConfigurationImportSelector.class)

- 在AutoConfigurationImportSelector 中通过SpringFactoriesLoader.loadFactoryNames()

  扫描所有jar类路径下 中

  - META-INF/spring.factories   是由 SpringFactoriesLoader.FACTORIES_RESOURCE_LOCATION配置的

  - ==META-INF/spring-autoconfigure-metadata.properties==是由AutoConfigurationMetadataLoader.PATH确定的

  - “spring.boot.enableautoconfiguration”是由EnableAutoConfiguration.ENABLED_OVERRIDE_PROPERTY确定的
  
  [spring.factories]: link/spring.factories
  
  
  
  ```properties
  # Initializers
  org.springframework.context.ApplicationContextInitializer=\
  org.springframework.boot.autoconfigure.SharedMetadataReaderFactoryContextInitializer,\
  org.springframework.boot.autoconfigure.logging.ConditionEvaluationReportLoggingListener
  
  # Application Listeners
  org.springframework.context.ApplicationListener=\
  org.springframework.boot.autoconfigure.BackgroundPreinitializer
  
  # Auto Configuration Import Listeners
  org.springframework.boot.autoconfigure.AutoConfigurationImportListener=\
  org.springframework.boot.autoconfigure.condition.ConditionEvaluationReportAutoConfigurationImportListener
  
  # Auto Configuration Import Filters
  org.springframework.boot.autoconfigure.AutoConfigurationImportFilter=\
  org.springframework.boot.autoconfigure.condition.OnBeanCondition,\
  org.springframework.boot.autoconfigure.condition.OnClassCondition,\
  org.springframework.boot.autoconfigure.condition.OnWebApplicationCondition
  
  # Auto Configure
  org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
  org.springframework.boot.autoconfigure.admin.SpringApplicationAdminJmxAutoConfiguration,\
  org.springframework.boot.autoconfigure.aop.AopAutoConfiguration,\
  org.springframework.boot.autoconfigure.amqp.RabbitAutoConfiguration,\
  org.springframework.boot.autoconfigure.batch.BatchAutoConfiguration,\
  org.springframework.boot.autoconfigure.cache.CacheAutoConfiguration,\
  org.springframework.boot.autoconfigure.cassandra.CassandraAutoConfiguration,\
  org.springframework.boot.autoconfigure.cloud.CloudServiceConnectorsAutoConfiguration,\
  org.springframework.boot.autoconfigure.context.ConfigurationPropertiesAutoConfiguration,\
  org.springframework.boot.autoconfigure.context.MessageSourceAutoConfiguration,\
  org.springframework.boot.autoconfigure.context.PropertyPlaceholderAutoConfiguration,\
  org.springframework.boot.autoconfigure.couchbase.CouchbaseAutoConfiguration,\
  org.springframework.boot.autoconfigure.dao.PersistenceExceptionTranslationAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.cassandra.CassandraDataAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.cassandra.CassandraReactiveDataAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.cassandra.CassandraReactiveRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.cassandra.CassandraRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.couchbase.CouchbaseDataAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.couchbase.CouchbaseReactiveDataAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.couchbase.CouchbaseReactiveRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.couchbase.CouchbaseRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.elasticsearch.ElasticsearchAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.elasticsearch.ElasticsearchDataAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.elasticsearch.ElasticsearchRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.elasticsearch.ReactiveElasticsearchRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.elasticsearch.ReactiveRestClientAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.jdbc.JdbcRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.jpa.JpaRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.ldap.LdapRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.mongo.MongoDataAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.mongo.MongoReactiveDataAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.mongo.MongoReactiveRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.mongo.MongoRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.neo4j.Neo4jDataAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.neo4j.Neo4jRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.solr.SolrRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.redis.RedisAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.redis.RedisReactiveAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.redis.RedisRepositoriesAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.rest.RepositoryRestMvcAutoConfiguration,\
  org.springframework.boot.autoconfigure.data.web.SpringDataWebAutoConfiguration,\
  org.springframework.boot.autoconfigure.elasticsearch.jest.JestAutoConfiguration,\
  org.springframework.boot.autoconfigure.elasticsearch.rest.RestClientAutoConfiguration,\
  org.springframework.boot.autoconfigure.flyway.FlywayAutoConfiguration,\
  org.springframework.boot.autoconfigure.freemarker.FreeMarkerAutoConfiguration,\
  org.springframework.boot.autoconfigure.gson.GsonAutoConfiguration,\
  org.springframework.boot.autoconfigure.h2.H2ConsoleAutoConfiguration,\
  org.springframework.boot.autoconfigure.hateoas.HypermediaAutoConfiguration,\
  org.springframework.boot.autoconfigure.hazelcast.HazelcastAutoConfiguration,\
  org.springframework.boot.autoconfigure.hazelcast.HazelcastJpaDependencyAutoConfiguration,\
  org.springframework.boot.autoconfigure.http.HttpMessageConvertersAutoConfiguration,\
  org.springframework.boot.autoconfigure.http.codec.CodecsAutoConfiguration,\
  org.springframework.boot.autoconfigure.influx.InfluxDbAutoConfiguration,\
  org.springframework.boot.autoconfigure.info.ProjectInfoAutoConfiguration,\
  org.springframework.boot.autoconfigure.integration.IntegrationAutoConfiguration,\
  org.springframework.boot.autoconfigure.jackson.JacksonAutoConfiguration,\
  org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration,\
  org.springframework.boot.autoconfigure.jdbc.JdbcTemplateAutoConfiguration,\
  org.springframework.boot.autoconfigure.jdbc.JndiDataSourceAutoConfiguration,\
  org.springframework.boot.autoconfigure.jdbc.XADataSourceAutoConfiguration,\
  org.springframework.boot.autoconfigure.jdbc.DataSourceTransactionManagerAutoConfiguration,\
  org.springframework.boot.autoconfigure.jms.JmsAutoConfiguration,\
  org.springframework.boot.autoconfigure.jmx.JmxAutoConfiguration,\
  org.springframework.boot.autoconfigure.jms.JndiConnectionFactoryAutoConfiguration,\
  org.springframework.boot.autoconfigure.jms.activemq.ActiveMQAutoConfiguration,\
  org.springframework.boot.autoconfigure.jms.artemis.ArtemisAutoConfiguration,\
  org.springframework.boot.autoconfigure.groovy.template.GroovyTemplateAutoConfiguration,\
  org.springframework.boot.autoconfigure.jersey.JerseyAutoConfiguration,\
  org.springframework.boot.autoconfigure.jooq.JooqAutoConfiguration,\
  org.springframework.boot.autoconfigure.jsonb.JsonbAutoConfiguration,\
  org.springframework.boot.autoconfigure.kafka.KafkaAutoConfiguration,\
  org.springframework.boot.autoconfigure.ldap.embedded.EmbeddedLdapAutoConfiguration,\
  org.springframework.boot.autoconfigure.ldap.LdapAutoConfiguration,\
  org.springframework.boot.autoconfigure.liquibase.LiquibaseAutoConfiguration,\
  org.springframework.boot.autoconfigure.mail.MailSenderAutoConfiguration,\
  org.springframework.boot.autoconfigure.mail.MailSenderValidatorAutoConfiguration,\
  org.springframework.boot.autoconfigure.mongo.embedded.EmbeddedMongoAutoConfiguration,\
  org.springframework.boot.autoconfigure.mongo.MongoAutoConfiguration,\
  org.springframework.boot.autoconfigure.mongo.MongoReactiveAutoConfiguration,\
  org.springframework.boot.autoconfigure.mustache.MustacheAutoConfiguration,\
  org.springframework.boot.autoconfigure.orm.jpa.HibernateJpaAutoConfiguration,\
  org.springframework.boot.autoconfigure.quartz.QuartzAutoConfiguration,\
  org.springframework.boot.autoconfigure.rsocket.RSocketMessagingAutoConfiguration,\
  org.springframework.boot.autoconfigure.rsocket.RSocketRequesterAutoConfiguration,\
  org.springframework.boot.autoconfigure.rsocket.RSocketServerAutoConfiguration,\
  org.springframework.boot.autoconfigure.rsocket.RSocketStrategiesAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.servlet.UserDetailsServiceAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.servlet.SecurityFilterAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.reactive.ReactiveSecurityAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.reactive.ReactiveUserDetailsServiceAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.rsocket.RSocketSecurityAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.saml2.Saml2RelyingPartyAutoConfiguration,\
  org.springframework.boot.autoconfigure.sendgrid.SendGridAutoConfiguration,\
  org.springframework.boot.autoconfigure.session.SessionAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.oauth2.client.servlet.OAuth2ClientAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.oauth2.client.reactive.ReactiveOAuth2ClientAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.oauth2.resource.servlet.OAuth2ResourceServerAutoConfiguration,\
  org.springframework.boot.autoconfigure.security.oauth2.resource.reactive.ReactiveOAuth2ResourceServerAutoConfiguration,\
  org.springframework.boot.autoconfigure.solr.SolrAutoConfiguration,\
  org.springframework.boot.autoconfigure.task.TaskExecutionAutoConfiguration,\
  org.springframework.boot.autoconfigure.task.TaskSchedulingAutoConfiguration,\
  org.springframework.boot.autoconfigure.thymeleaf.ThymeleafAutoConfiguration,\
  org.springframework.boot.autoconfigure.transaction.TransactionAutoConfiguration,\
  org.springframework.boot.autoconfigure.transaction.jta.JtaAutoConfiguration,\
  org.springframework.boot.autoconfigure.validation.ValidationAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.client.RestTemplateAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.embedded.EmbeddedWebServerFactoryCustomizerAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.reactive.HttpHandlerAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.reactive.ReactiveWebServerFactoryAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.reactive.WebFluxAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.reactive.error.ErrorWebFluxAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.reactive.function.client.ClientHttpConnectorAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.reactive.function.client.WebClientAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.servlet.DispatcherServletAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.servlet.ServletWebServerFactoryAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.servlet.error.ErrorMvcAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.servlet.HttpEncodingAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.servlet.MultipartAutoConfiguration,\
  org.springframework.boot.autoconfigure.web.servlet.WebMvcAutoConfiguration,\
  org.springframework.boot.autoconfigure.websocket.reactive.WebSocketReactiveAutoConfiguration,\
  org.springframework.boot.autoconfigure.websocket.servlet.WebSocketServletAutoConfiguration,\
  org.springframework.boot.autoconfigure.websocket.servlet.WebSocketMessagingAutoConfiguration,\
  org.springframework.boot.autoconfigure.webservices.WebServicesAutoConfiguration,\
  org.springframework.boot.autoconfigure.webservices.client.WebServiceTemplateAutoConfiguration
  
  # Failure analyzers
  org.springframework.boot.diagnostics.FailureAnalyzer=\
  org.springframework.boot.autoconfigure.diagnostics.analyzer.NoSuchBeanDefinitionFailureAnalyzer,\
  org.springframework.boot.autoconfigure.flyway.FlywayMigrationScriptMissingFailureAnalyzer,\
  org.springframework.boot.autoconfigure.jdbc.DataSourceBeanCreationFailureAnalyzer,\
  org.springframework.boot.autoconfigure.jdbc.HikariDriverConfigurationFailureAnalyzer,\
  org.springframework.boot.autoconfigure.session.NonUniqueSessionRepositoryFailureAnalyzer
  
  # Template availability providers
  org.springframework.boot.autoconfigure.template.TemplateAvailabilityProvider=\
  org.springframework.boot.autoconfigure.freemarker.FreeMarkerTemplateAvailabilityProvider,\
org.springframework.boot.autoconfigure.mustache.MustacheTemplateAvailabilityProvider,\
  org.springframework.boot.autoconfigure.groovy.template.GroovyTemplateAvailabilityProvider,\
org.springframework.boot.autoconfigure.thymeleaf.ThymeleafTemplateAvailabilityProvider,\
  org.springframework.boot.autoconfigure.web.servlet.JspTemplateAvailabilityProvider

  ```
  
  - 像这样的 org.springframework.boot.autoconfigure.EnableAutoConfiguration类都是容器中的一个组件，都加入到容器中；用他们来做自动配置
  - 每一个自动配置类继续自动配置功能（xxxAutoCOnfiguration类  //TODO未验证）
  - 所有在配置文件中能配置的属性都是在xxxProperties类中封装//TODO 未验证
  
  
  
  可以在配置文件中 **==debug=true==**启用springbootdebug模式，分析自动配置类，查看自动配置报告。
  
  
  
  
  
  