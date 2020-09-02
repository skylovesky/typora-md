## 生命周期



```javascript
const app = new Vue({
    el:'#app',
    data:{
        
    },
    methods:{
        
    },
    computed:{//计算属性
        
    },
    filters:{//过滤器
        
    },
    components:{//注册组件
        my-temp:temp//my-temp是使用组件时的标签名
    }
    
})
```





## 基础

#### v-once

#### v-html

#### v-text

#### v-pre

#### v-cloak



#### v-bind

- 动态绑定属性
- 缩写:   **:**

#### v-for

#### v-on

- 语法糖: @

#### key

#### v-show

#### v-if

#### v-else-if

#### v-else

#### v-model

- 修饰符
  - .lazy
  - .number
  - .trim



## 函数的增强写法

```typescript
const obj={
    run:function(){
        
    }
}

const o={
    run(){
        
    }
}
```

## 组件化

### 使用步骤

1. 创建组件构造器（全局注册  |  局部注册）

   - Vue.extend()

     ```javascript
     const temp = Vue.extend({
         template:`<html></html>`,
         components:{//注册子组件
             newTemp:new-Temp
         }
     })
     ```

     

2. 注册组件

   - Vue.component()//全局注册  注册了 **全局组件**:可以在多个vue实例下面使用

     ```javascript
     Vue.component('my-temp',temp)
     ```

     

3. 使用组件

   - 在Vue实例的作用范围使用组件

     ```html
     <my-temp></my-temp>
     ```



- 创建组件可以和注册组件同时：

  ```javascript
  Vue.component('my-temp',{
      template:``,
      data(){//组件自己的data函数
          return{}
      },
      methods:{//组件自己的methods
          
      }
  })
  ```

  







### 组件模板抽离

```html
<template id='tempId'>
		....
</template>

<script>
	Vue.component('temp',{
        template:'#tempId'
    })
</script>

```



### 父子组件之间的通信

- 通过props（properties）向子组件传递数据

  - 字符串数组，数组中的字符串就是传递时的名称

    ```html
    <div>
        <cpn v-bind:cmovies="movies"></cpn>
    </div>
    
    <script>
    	const cpn = {
            templat:`#cpn`,
            props:['cmovies'],//{ cmovies:Array}//可以用对象的形式接收，并且验证数据类型
            /**
            	props:{
            		comvies:Array,
            		message:{
            			type:Stirng,
            			default:'abc',
            			require:true//必须传值
            		}
            	}
            */
            data(){
                
            },
            methods:{
                
            }
        } 
        
    </script>
    
    ```

    

  - 对象，对象可以设置传递是的类型，也可以设置默认值等

- 通过事件向父组件发送消息（$emit Events）

```javascript
<script>
	const cpn = {
        templat:`#cpn`,
        props:['cmovies'],//{ cmovies:Array}//可以用对象的形式接收，并且验证数据类型
        /**
        	props:{
        		comvies:Array,
        		message:{
        			type:Stirng,
        			default:'abc',
        			require:true//必须传值
        		}
        	}
        */
        data(){
            
        },
        methods:{
            bitClick(item){
                this.$emit('itemclick',item)//子组件监听  bitClick事件，并向父组件发送一个itemclick事件（传递的参数是item）;<cpu></cpu>标签中这样接收 @itemclick="cpnClick(item)"
            }
        },
        watch:{
            
        }
    } 
    
</script>
```



### 访问子组件

#### $children

#### $refs

```html
<cpn ref="nameOrId"></cpn>
```



```javascript
this.$refs.nameOrId. 
```

### 访问父组件

#### $parent

#### $root

```javascript
this.$root  //访问根组件
```



### 插槽slot

默认slot

具名slot







#### 计算属性

- computed

  ```javascript
  computed:{
      title:function(){
          return ''+''
      }
  }
  ```

  

#### 事件监听

- v-on  

  - 语法糖 ：@

  - 修饰符

    - .stop  阻止事件冒泡   =event.stopPropagation()

      ```javascript
      @click.stop="btnClick"(写在里面的事件里)
      ```

    - .prevent 阻止默认事件 = event.preventDefault()

      ```javascript
      @click.prevent
      ```

    ![](images\001.PNG)

