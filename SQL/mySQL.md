# MySQL

##  命令

##### 连接数据库

mysql  -h  localhost  -u  root  -p	

##### 创建数据库

create  database  company 

##### 连接数据库

use  company;					

 

##### 显示表

show  tables;

describe  表名

##### 查看表结构

desc  表名;

##### 查看表详细结构				

show  create  table 表名;  



##### 修改表名 

alter  table  <旧表名>  rename  [to]  <新表名> ; 

##### 修改字段数据类型

alter  table  <表名>  modify  <字段名>  <数据类型>;

##### 修改字段名

alter  table  <表名>  change  <旧字段名>  <新字段名>  <数据类型>; 

##### 添加字段

alter  table  <表名>  add  <新字段名>  <数据类型>;  

##### 删除字段

alter  table  <表名>  drop  <字段名>;  

##### 更改表储存引擎

alter  table  <表名>  engine=<更改后储存引擎名称>; 

##### 删除表

drop  table  [if exists] 表1,表2 ... ;

 

 

 

#### 存储过程

 

***\*create  procedure  name ([proc_parameter])  [characteristics ... ]  routine_body\**** 

***\*create  procedure\**** 	创建存储过程关键字

***\*name\**** 				存储过程名称

***\*proc_parameter\****  	指定存储过程参数列表 ***\*[IN | OUT | IN_OUT ]  parameter_name  type\****

***\*characteristics\**** 		指定存储过程特性

l LANGUAGE  SQL  

说明存储过程中使用的是sql语言编写的

l NOT  DETERMINISTIC 

是否确定性的输入就是确定性的输出，默认是NOT DETERMINISTIC，只	对于同样的输入，输出也是一样的，当前这个值还没有使用

l CONTAINS SQL 
提供子程序使用数据的内在信息，这些特征值目前提供给服务器，并没有根据这些特征值来约束过程实际使用数据的情况，说白了就是没有使用的
包括以下四种选择
	1.CONTAINS SQL表示子程序不包含读或者写数据的语句
	2.NO SQL 表示子程序不包含sql
	3.READS SQL DATA 表示子程序包含读数据的语句，但是不包含写数据的语句
	4.MODIFIES SQL DATA 表示子程序包含写数据的语句。

***\*routine_body  SQL代码内容  BEGIN...END\****

 

 

 

 

#### 数学函数

##### ABS()	绝对值

l		select abs(2),abs(-9.2),abs(-4);

##### PI()		圆周率

l		select abs(-3),PI();

##### SQRT() 	平方根函数

l	select sqrt(9),sqrt(40),sqrt(-49);

+---------+-------------------+-----------+

| sqrt(9) | sqrt(40)      | sqrt(-49) |

+---------+-------------------+-----------+

|    3 | 6.324555320336759 | NULL    |

+---------+-------------------+-----------+

##### MOD(x,y)	取余函数

返回不小于x的最小整数

##### CEIL(x)  CEILING(x)		取整

##### FLOOR(x)				取整	

返回不大于x的最小整数、

##### RAND()				随机数

返回(0<=v<=1.0)之间的随机数

##### RAND(x)				随机数

相同的x返回的随机数是相同的  0<=v<=1.0

##### ROUND(x)			取整

对x四舍五入

##### ROUND(x,y)				 对x四舍五入 

保留小数点后y位 y可以为负数

##### TRUNCATE(x,y)

对x操作 舍去小数点后面y位,y可以为负数

##### SIGN(x)

取x的符号,  -1,	0, 	1

##### POW(x,y) 和 POWER(x,y)

幂函数  x的y次乘方  y可以为负数(倒数)

##### EXP(x)

e的乘方

 

# 高性能MySQL笔记 糙

## 并发控制

### 锁（lock）

l 读写锁

n 读锁（read lock）== 共享锁（shared lock）

n 写锁（write lock）==排他锁（exclusive lock）

l 锁粒度

n 表锁（table lock）

n 行级锁（row lock）

### 事务（ACID）

l 原子性（Atomicity）

l 一致性（Consistency）

l 隔离性（Isolation）

l 持久性（Durability）

 

### 隔离级别

### 死锁

 

***\*show  VARIABLES;\****

 

***\*show table status;\****

 

mysql -uroot -p -h127.0.0.1 -P3309

##### innodb_buffer_pool_size

show global variables like 'innodb_buffer_pool_size';

 

SET GLOBAL innodb_buffer_pool_size= 524288000;

 

 

ALTER TABLE `sys_water` ADD INDEX sys_index_dataTime ( `dataTime` )

 

 

show global variables like 'max_allowed_packet';

 

SET GLOBAL max_allowed_packet= 2524971008 ;

 

 

show global variables like 'wait_timeout';

 

SET GLOBAL wait_timeout= 2880000;

 

 

show global variables like 'interactive_timeout';

 

SET GLOBAL interactive_timeout= 2880000;