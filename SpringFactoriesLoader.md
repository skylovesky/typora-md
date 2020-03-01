# SpringFactoriesLoader

```java
package org.springframework.core.io.support;

public final class SpringFactoriesLoader {
    
    public static final String FACTORIES_RESOURCE_LOCATION = "META-INF/spring.factories";
    
    public static List<String> loadFactoryNames(Class<?> factoryType, 
                                                @Nullable ClassLoader classLoader) {
		String factoryTypeName = factoryType.getName();
        
		return loadSpringFactories(classLoader).
            getOrDefault(factoryTypeName, Collections.emptyList());
	}
    
}
```

先后获取到factoryTypeName

- org.springframework.context.ApplicationContextInitializer
- org.springframework.context.ApplicationListener
- org.springframework.boot.SpringApplicationRunListener
- org.springframework.boot.env.EnvironmentPostProcessor
- org.springframework.boot.env.PropertySourceLoader
- org.springframework.boot.SpringBootExceptionReporter
- org.springframework.boot.diagnostics.FailureAnalyzer
- org.springframework.beans.BeanInfoFactory
- org.springframework.boot.autoconfigure.EnableAutoConfiguration
- org.springframework.boot.autoconfigure.AutoConfigurationImportFilter
- org.springframework.boot.autoconfigure.AutoConfigurationImportListener
- org.springframework.boot.autoconfigure.template.TemplateAvailabilityProvider
- org.springframework.boot.diagnostics.FailureAnalysisReporter



其中org.springframework.boot.autoconfigure.EnableAutoConfiguration的研究在

[Spring boot启动]: SpringBoot（一）.md

