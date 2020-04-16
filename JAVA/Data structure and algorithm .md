# 数据结构和算法



### KMP算法

> KMP 算法是 D.E.Knuth、J,H,Morris 和 V.R.Pratt 三位神人共同提出的，称之为 Knuth-Morria-Pratt 算法，简称 KMP 算法。该算法相对于 Brute-Force（暴力）算法有比较大的改进，主要是消除了主串指针的回溯，从而使算法效率有了某种程度的提高。

[KMP算法]: https://www.sohu.com/a/336648975_453160

![](images/KWP/ac42a5a0d622448d829c103dd090a4b4.jpeg)
![](images/KWP/d1dc04a6898f47afbcf0a282282c8783.jpeg)
![](images/KWP/2fd0925dae1d420aaaeb0da6d228730d.jpeg)
![](images/KWP/1765ddd86e6a4584adbc8598fac1a15d.jpeg)
![](images/KWP/a72fb351b5e74bc4877c90aeddad2841.jpeg)
![](images/KWP/28faa3308fcc4effb63f9f1870a7b324.jpeg)
![](images/KWP/ca194c56d14b4e14b5aa454715088246.jpeg)
![](images/KWP/b69d113c64f74fb885c225515babf6e9.gif)
![](images/KWP/29fd912afed9427ea29f352e0490d23d.jpeg)
![](images/KWP/cfed893e7abd42d6b7ada497713af1b3.jpeg)
![](images/KWP/4db8a32f54b44bb184226090316eacf2.jpeg)
![](images/KWP/8d49e933951744b89c0ee59cbdbde292.jpeg)
![](images/KWP/38e37abb41504659895a6bb6413aff9e.jpeg)
![](images/KWP/8252250502c1437f93a3316d181dd961.jpeg)
![](images/KWP/725a6dbaa66a42e595195177598fcf1a.jpeg)
![](images/KWP/8a9eabcd6b21421cbb669cd9ab69f048.jpeg)
![](images/KWP/8f20e83b70254fa6a026f7368852e585.jpeg)
![](images/KWP/325ae835341649ea9e887badc01b6c39.jpeg)
![](images/KWP/562a2fe4a7394d788e92abb81da547b2.jpeg)
![](images/KWP/e0fae3c781254aefb77db951a056273f.jpeg)
![](images/KWP/d18ebeab782f4fb29ef9f62cb301ba56.jpeg)
![](images/KWP/0e51efda4a3c41458a68fd8b3f9db07f.gif)
![](images/KWP/af3f74cb6ba948b78591eb61a361a0e6.jpeg)
![](images/KWP/47782779a36c45e4b777fb47587ce539.jpeg)
![](images/KWP/3b7f9375bb77431484b67da7101b5529.gif)
![](images/KWP/97e5f54015654ca49b87c61cbab2550a.jpeg)
![](images/KWP/bc88ed31c9c144358920ee0677eb8d65.jpeg)
![](images/KWP/cdd2b831db86416d862635e1ee73c13a.gif)

![](images/KWP/2c398b5dabb4469da60dcd3773b630b9.jpeg)
![](images/KWP/2ce3570cff1f4a999d2c91fb85375caf.jpeg)
![](images/KWP/06b731b37e244248bc0e622b0738c063.jpeg)
![](images/KWP/710e385d19e545edb9db89c123c8b365.gif)
![](images/KWP/b0c9cb5e429a468eb689eb0593ed83d6.jpeg)
![](images/KWP/e05b228d070843afa82bdbf3562c1d2f.jpeg)
![](images/KWP/96333bdca42948249e4a7a6fcb0a7079.jpeg)
![](images/KWP/1765328d9761443d989166298c99757b.jpeg)
<p style="text-align: left;"><span>所以公共前后缀的条件为：</span></p> 
<p style="text-align: left;"><span>1、最长的前<span>后</span>缀</span></p> 
<p style="text-align: left;"><span>2、长度小于指针前所有字符长度</span></p> 
![](images/KWP/e8cfb756561245d4b9efe187efdc28cb.jpeg)
![](images/KWP/9eb33f40c5254fbfbb1b0b1dd6a4af9d.jpeg)
<p style="text-align: left;"><span>然后移动，使得公共前缀移动到后缀的位置</span></p> 
![](images/KWP/260cb9e4dce24935bb6ee47e54dbd6d1.gif)
<p style="text-align: left;"><span>然后继续比较！</span></p> 
![](images/KWP/aaa54cb240c148809b176494162c203c.gif)

**上面这个图有问题**；

- 前缀A  后缀A
- 前缀AB  后缀AA  --这里是不是可以确定公共前后缀为A啊，此时是不是可以不用再继续比较，直接移动模式串就可以了

![](images/KWP/0a8425bbee414c038b51a41f7cc98060.jpeg)
![](images/KWP/618a9905e2b64b8eb0b656186ae2c3aa.jpeg)
![](images/KWP/1712792fb9224d00bb298e8e880bd429.jpeg)
![](images/KWP/e01d2081b438482288c93e842fd62198.jpeg)
![](images/KWP/fbcf32133b224a0f87b8d04d208999f0.jpeg)
![](images/KWP/5c6a495d5d934bb48d0f59d588038f3a.jpeg)
![](images/KWP/91fe7748a72d430c8698dc2f010c94d9.jpeg)
![](images/KWP/5372e24957a94c9dbfa46d371e951a47.jpeg)
![](images/KWP/ef0a74b9baab4f759332d6ea1e2f9ae2.jpeg)
![](images/KWP/ab452ee6a2a24162bf18f063301ed94b.jpeg)
![](images/KWP/b127391b275944b592fe92097e566033.jpeg)
![](images/KWP/0265cd034a8b495087f246d1c272364f.jpeg)

![](images/KWP/7afad62a72ea4a24a5679bd286b0696a.jpeg)
![](images/KWP/ee14aefa94a041198c3fb95cb464630b.gif)
![](images/KWP/c393287772c34a28940c6dd272f31c4f.jpeg)
![](images/KWP/c5ec4cf90bd84b6b92844dc3edc6ee89.jpeg)
![](images/KWP/4228ed1a44d54b15a4a4eee81c554f3e.jpeg)
![](images/KWP/876e2ec8d4564ec1a3e15e9e1eb8c9b4.jpeg)
![](images/KWP/bce5a6ae3abf41b78b4e010a99d893c5.jpeg)
![](images/KWP/e1d56f95c3d94ffc9fb268e2d2531f5b.jpeg)
![](images/KWP/096aec7f93ff4e7abe1986e96f0cb089.jpeg)
![](images/KWP/91d6084272eb448c83a1038fe1dab944.jpeg)
![](images/KWP/6ae90b0f4a97466e8cbd1d8681bc5dac.jpeg)
![](images/KWP/ec3143189ba64fcfb2324922353e6cca.jpeg)
![](images/KWP/f3fa809f25f243a5a2f0d37d242dda54.jpeg)
![](images/KWP/c9545b8069e64f31886da41174d9430c.jpeg)
![](images/KWP/c32e50663d184c0397f8bbe882c6e88d.gif)

<p style="text-align: left;"><span>老板我要抢答，第四位结果如下：</span></p> 
![](images/KWP/804d9e0711c449b99f356c6e8ca99fae.jpeg)
<p style="text-align: left;"><span>老板，我要接着抢答，第五位结果如下：</span></p> 
![](images/KWP/a32ab99b1c7547979e087cc0571232db.jpeg)
<p style="text-align: left;"><span>老板，我要再接着抢答~下面详细说一下吧，包括第六位的详细分析过程</span></p> 
![](images/KWP/4c3dddeedb6f4fc6b6c834ef026e9bd0.jpeg)
<p style="text-align: left;"><span>先找到公共前后缀</span></p> 
![](images/KWP/0ddbcb66c18c4509947fefa78a77e082.gif)
<p style="text-align: left;"><span>也就是模式串串的第四位与主串串的当前位置作比较</span></p> 
![](images/KWP/5ce16f977e1c4107a7d59b13ea8fbc2b.jpeg)
![](images/KWP/5c926a322efd4ba0954a9fb5f4d180af.jpeg)
![](images/KWP/6d5e579454b94578aa79205319e07fca.jpeg)
![](images/KWP/269fa0f480cd45db8fefbf2feeacd9d3.jpeg)
![](images/KWP/256f1133fcd246e789c99ec40eb873dd.jpeg)
![](images/KWP/63fb7fe17b2046c1a1f49d76b9e73ce9.gif)
![](images/KWP/8e3a45bb043c42e9b180f5fbedb978b7.jpeg)
![](images/KWP/e0f980510e6942858bb061ad776d3d6f.gif)
<p style="text-align: left;"><span>转换结果图如下：</span></p> 
![](images/KWP/f6d894a8fd564135875e3f263219f3d5.jpeg)
<p style="text-align: left;"><span>嗯！！！我终于明白了！比如模式串串1位置上发生了不匹配，则按照我们刚才定义的规则进行执行“1号位与主串串的下一位进行比较”</span></p> 
![](images/KWP/4ea24ae7a25d47bf808e22173e510819.jpeg)
![](images/KWP/885da81439ca48d89f5d3e8f804c643e.jpeg)

<p style="text-align: left;"><span>瞬间把刚才分析的图拿出来！</span></p> 
<p style="text-align: center;"><span>??看毛片神图</span></p> 
![](images/KWP/72bc3b8aeca849d9aa4fb7dd754918f4.jpeg)
![](images/KWP/794ff63bd58641f1b71466c9ce7c5d39.jpeg)
![](images/KWP/7c52a730bf1f4cd6a7e6062f3099f243.jpeg)
![](images/KWP/389bbe445a214545b13bc30fd798dce0.jpeg)
![](images/KWP/4ecf1fa967fe49e3a9871ca61dc6cac4.jpeg)



#### 总结

1. 模式串和主串比较
   - 如果有全字符串相同，比较完成，咱就不说这个了
2. 标记记录模式串和主串不同的位置X
3. 相同的字符串，找出模式串中找出最长（但小于模式串长度）公共的前后缀
4. 移动模式前缀串（X位置之前的串变为前缀串）
5. 再次循环以上操作，直到匹配到字符串



#### 思考原理

........CM（前）...（中间串）..CM（后）X.....

下一步CM（前）移动到 CM（后）

中间串中是否有CMX，这与前一步的比较有关

前后串的移动肯定不会超出模式串长度

模式串长度肯定小于被比较串和主串长度

所以每次移动不会超出预期长度

而上一次的比较，













### 分治算法

### 回溯算法

### DFS算法

#### 贪心算法



# 算法案例

### 八皇后游戏

### 马踏棋盘

### 汉诺塔游戏