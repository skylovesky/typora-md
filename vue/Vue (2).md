## ES6导入导出

info.js

```JavaScript
//es6语法导出
export default {    name:'zzz',    age:24,}
```

mathUtils.js

```javascript
export function add(num1,num2) {
    return num1+num2
}
export function mul(num1,num2) {
    return num1*num2
}
```

main.js

```javascript
import info from './info.js'
import {add,mul} from "./mathUtils.js";


console.log(info.name)
console.log(info.age)
console.log(add(1,3))
```

html

```html
<script src="js/main.js" type="module"></script>
```

- 不要和其他导入导出格式搞混
- 注意  **type="module"**





**npm install -g browserify**