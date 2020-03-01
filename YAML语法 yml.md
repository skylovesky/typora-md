# YMAL语法  yml

 它被用来描述我们的**配置管理**语言.  它实质上是一种通用的数据串行化格式 

###  基本语法规则 

> - **大小写敏感**
> - **使用缩进表示层级关系**
> - **缩进时不允许使用Tab键，只允许使用空格。**
> - **缩进的空格数目不重要，只要相同层级的元素左侧对齐即可**

 `#` 表示注释，从这个字符一直到行尾，都会被解析器忽略。 

###  YAML 支持的数据结构有三种

> - **对象：键值对的集合，又称为映射（mapping）/ 哈希（hashes） / 字典（dictionary）**
> - **数组：一组按次序排列的值，又称为序列（sequence） / 列表（list）**
> - **纯量（scalars）：单个的、不可再分的值**



## 具体数据结构

### 对象

对象的一组键值对，使用冒号结构表示。

yml:

```yaml
animal: pets
```

Javascript:

```javascript
{ animal: 'pets' }
```

 Yaml 也允许另一种写法，将所有键值对写成一个行内对象。 

```yaml
hash: { name: Steve, foo: bar } 
```

 JavaScript  :

```javascript
{ hash: { name: 'Steve', foo: 'bar' } }
```

### 数组

 一组连词线开头的行，构成一个数组。

```yaml
- Cat
- Dog
- Goldfish
```

 数据结构的子成员是一个数组，则可以在该项下面缩进一个空格 

```yaml
-
 - Cat
 - Dog
 - Goldfish
```

 数组也可以采用行内表示法 

```yaml
animal: [Cat, Dog]
```

### 复合结构

 对象和数组可以结合使用，形成复合结构

```yaml
languages:
 - Ruby
 - Perl
 - Python 
websites:
 YAML: yaml.org 
 Ruby: ruby-lang.org 
 Python: python.org 
 Perl: use.perl.org 
```

### 纯量

 纯量是最基本的、不可再分的值 

> - 字符串
> - 布尔值
> - 整数
> - 浮点数
> - Null
> - 时间
> - 日期

数值直接以字面量的形式表示

```yaml
number: 12.30
```

布尔值用`true`和`false`表示。

```yaml
isSet: true
```

布尔值用`true`和`false`表示。

```yaml
isSet: true
```

YAML 允许使用两个感叹号，强制转换数据类型。

```yaml
e: !!str 123
f: !!str true
```

### 字符串

字符串是最常见，也是最复杂的一种数据类型。

字符串默认不使用引号表示。

```yaml
str: 这是一行字符串
```

如果字符串之中包含空格或特殊字符，需要放在引号之中

```yaml
str: '内容： 字符串'
```

单引号和双引号都可以使用，双引号不会对特殊字符转义。

```yaml
s1: '内容\n字符串'
s2: "内容\n字符串"
```

单引号之中如果还有单引号，必须连续使用两个单引号转义。

```jyaml
str: 'labor''s day' 
```

