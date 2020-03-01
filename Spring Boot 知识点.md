# Spring Boot 相关知识点

### @import

允许通过它引入 @Configuration 注解的类 (java config)， 引入ImportSelector接口(这个比较重要， 因为要通过它去判定要引入哪些@Configuration) 和 ImportBeanDefinitionRegistrar 接口的实现，  也包括 @Component注解的普通类。

但是如果要引入另一个xml 文件形式配置的 bean, 则需要通过 @ImportResource 注解。

```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Import {

	/**
	 * {@link Configuration @Configuration}, {@link ImportSelector},
	 * {@link ImportBeanDefinitionRegistrar}, or regular component classes to import.
	 */
	Class<?>[] value();

}
```

![image-20200219100430856](images\spring-boot1-001.png)







### AnnotatedElement “被注解的元素” 

 		这个接口（AnnotatedElement）的对象代表了在当前JVM中的一个“被注解元素”（可以是Class，Method，Field，Constructor，Package等）。 

​		所有实现了这个接口的“元素”都是可以“被注解的元素”。使用这个接口中声明的方法可以读取（**通过Java的反射机制**）“被注解元素”的注解。这个接口中的所有方法返回的注解都是不可变的、并且都是可序列化的。这个接口中所有方法返回的数组可以被调用者修改，而不会影响其返回给其他调用者的数组。

> ```java
> package java.lang.reflect;
> 
> import java.lang.annotation.Annotation;
> import java.lang.annotation.AnnotationFormatError;
> import java.lang.annotation.Repeatable;
> import java.util.Arrays;
> import java.util.LinkedHashMap;
> import java.util.Map;
> import java.util.Objects;
> import java.util.function.Function;
> import java.util.stream.Collectors;
> import sun.reflect.annotation.AnnotationSupport;
> import sun.reflect.annotation.AnnotationType;
> 
> public interface AnnotatedElement {
>     //如果指定类型的注解出现在当前元素上，返回true，否则将返回false。这种方法主要是为了方便地访问一些已知的注解。
>     default boolean isAnnotationPresent(Class<? extends Annotation> annotationClass) {
>         return getAnnotation(annotationClass) != null;
>     }
>     
>     //如果在当前元素上存在参数所指定类型（annotationClass）的注解，则返回对应的注解，否则将返回null。
>      <T extends Annotation> T getAnnotation(Class<T> annotationClass);
>     
>     //返回在这个元素上的所有注解。如果该元素没有注释，则返回值是长度为0的数组。该方法的调用者可以自由地修改返回的数组;它不会对返回给其他调用者的数组产生影响。
>      Annotation[] getAnnotations();
>     
>     
>     //返回与该元素相关联的注解。如果没有与此元素相关联的注解，则返回值是长度为0的数组。这个方法与getAnnotation(Class)的区别在于，该方法检测其参数是否为可重复的注解类型(JLS 9.6)，如果是，则尝试通过“looking through”容器注解来查找该类型的一个或多个注解。该方法的调用者可以自由地修改返回的数组;它不会对返回给其他调用者的数组产生影响。
>      default <T extends Annotation> T[] getAnnotationsByType(Class<T> annotationClass) {
>          T[] result = getDeclaredAnnotationsByType(annotationClass);
>          if (result.length == 0 && // Neither directly nor indirectly present
>              this instanceof Class && // the element is a class
>              AnnotationType.getInstance(annotationClass).isInherited()) { // Inheritable
>              Class<?> superClass = ((Class<?>) this).getSuperclass();
>              if (superClass != null) {
>                  // Determine if the annotation is associated with the
>                  // superclass
>                  result = superClass.getAnnotationsByType(annotationClass);
>              }
>          }
> 
>          return result;
>      }
>     
>     //如果参数中所指定类型的注解是直接存在于当前元素上的，则返回对应的注解，否则将返回null。这个方法忽略了继承的注解。(如果没有直接在此元素上显示注释，则返回null。)
>     default <T extends Annotation> T getDeclaredAnnotation(Class<T> annotationClass) {
>          Objects.requireNonNull(annotationClass);
>          // Loop over all directly-present annotations looking for a matching one
>          for (Annotation annotation : getDeclaredAnnotations()) {
>              if (annotationClass.equals(annotation.annotationType())) {
>                  // More robust to do a dynamic cast at runtime instead
>                  // of compile-time only.
>                  return annotationClass.cast(annotation);
>              }
>          }
>          return null;
>      }
>     
>     
>     //如果参数中所指定类型的注解是直接存在或间接存在于当前元素上的，则返回对应的注解。这种方法忽略了继承的注释。如果没有直接或间接地存在于此元素上的指定注解，则返回值是长度为0的数组。这个方法和getDeclaredAnnotation(Class)的区别在于，这个方法检测它的参数是否为可重复的注释类型(JLS 9.6)，如果是，则尝试通过“looking through”容器注解来查找该类型的一个或多个注解。该方法的调用者可以自由地修改返回的数组;它不会对返回给其他调用者的数组产生影响。
>     default <T extends Annotation> T[] getDeclaredAnnotationsByType(Class<T> annotationClass) {
>         Objects.requireNonNull(annotationClass);
>         return AnnotationSupport.
>             getDirectlyAndIndirectlyPresent(
>             Arrays.stream(getDeclaredAnnotations()).collect(
>                 Collectors.toMap(Annotation::annotationType,Function.identity(),
> 				((first,second) -> first),LinkedHashMap::new)
>             ),annotationClass);
>     }
>     
>     //返回直接出现在这个元素上的注解。这种方法忽略了继承的注解。如果在此元素上没有直接存在的注解，则返回值是长度为0的数组。该方法的调用者可以自由地修改返回的数组;它不会对返回给其他调用者的数组产生影响。
>     Annotation[] getDeclaredAnnotations();
> }
> 
> ```

![image-20200219102040294](images\spring-boot-0-002.png)

#### 接口

1. AnnotatedArrayType （被注解的数组类型）
2. AnnotatedParameterizedType （被注解的参数化类型）
3. AnnotatedType （被注解的类型）
4. AnnotatedTypeVariable （被注解的类型变量）
5. AnnotatedWildcardType （被注解的通配类型）
6. GenericDeclaration （通用声明，用于表示声明型元素，如：类、方法、构造器等）
7. TypeVariable<D> （类型变量）

#### 实现类

1. AccessibleObject（可访问对象，如：方法、构造器、属性等）
2. Class（类，就是你用Java语言编程时每天都要写的那个东西）
3. Constructor（构造器，类的构造方法的类型）
4. Executable（可执行的，如构造器和方法）
5. Field（属性，类中属性的类型）
6. Method（方法，类中方法的类型）
7. Package（包，你每天都在声明的包的类型）
8. Parameter（参数，主要指方法或函数的参数，其实是这些参数的类型）