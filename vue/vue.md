### 生命周期



### 指令

#### 插值语法

- v-once
- v-html
- v-text
- v-pre

- v-cloak

#### 绑定属性

- v-bind
  - 动态绑定属性
  - 缩写:   **:**



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

