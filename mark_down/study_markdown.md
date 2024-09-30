## vscode小技巧

用ctrl+k ，松开再按z键就可以切换沉浸式模式  
既可以用两次esc退出，也可以用进入该模式相同按键

一个#加空格是一级标题
# 这是一级标题
两个#加空格是二级标题
## 这是二级标题
三个#加空格是三级标题
### 这是三级标题

## 常用

直接写就是正文
换一行是不能直接换一行的  
如果想要换一行要么在前面的一段后面加两个空格（但是还是属于一段）

要么与上一行之间再相隔一行（另起一段）

**加粗就在字的两端加两个*号**，字体选中再ctrl+B可以快捷切换加粗

*斜体就是在字的两端加一个*号*，字体选中再ctrl+i就可以快捷切换斜体

---

打三个-号就是分割线

引用：在需要有用的一行前加上>

> 这是一个引用


## 列表

### 列表分为有序列表和无序列表

有序列表就直接在前面加1.再加一个空格 如
1. 列表第一项
2. 列表第二项
   1. 还支持用tab缩进产二级列表
3. 用shift+tab可以吧二级列表缩进到上一级
1. 不一定要按数字顺序编写，其他数字依然会编译到正确的数字

需要重新写一个列表时需要在两个列表之间加一段正文

1. 新列表
2. 接下来

### 无序列表

用*加空格或者-加空格或者+加空格

* 这是没有编号的列表
- 用-号也可以
+ 用加号也可以
  
## 图片

图片可以直接粘贴复制进来 用ctrl + Alt + V  
![图片](2023-03-24-21-22-14.png)  
*图片*  
图片会直接复制到当下文件下，也可以直接写!+[]+()，将图片名字写在小括号里面就可以将图片加载到文档中，除了文件名，其他的描述和caption都是可选的。描述就是你把鼠标停留在图片上显示出来的字，也是图片无法加载时显示的文字。

再在图片下写一个斜体就可以变成图片小标题

## 公式

支持Latex格式公式，只需要用$$+换行+内容+换行+$$  

在斜杠\\后面写下特殊的符号、希腊字母等，  
_和^对应上下标，  
用{}将符号变为一组，公式换行可以用双斜杠\\,  
可以用可以输入斜杠\加tag{1}来给公式编号
如：
$$
\tag{2} \lim_{x \to \infin}\frac{sin(t)^2}{csin(t)}=1 \\ \lim_{x \to \infin}\frac{sin(t)}{csin(t)}=1 
$$

也可以在一段文字中插入公式，用$+换行+内容+换行+$来写文中公式，不过建议在第一个$前加一个空格保证在其他文本编辑器中能正常加载，如 $\lim_{x }{}{}$，这是一个公式

[关于公式的文章](https://math.meta.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference)

## 表格

表格中每一项是由竖线|隔开，第一行固定为表头，第二行必须为几个-号，多少行就用多少个-，并用竖线|隔开，剩下的就是表格内容，  
在对应列的减号-打冒号就是对应的对齐方式，在减号左边打冒号就是该列左对齐，在减号右边打冒号就是右对齐  两边有冒号:就是居中对齐  
如果觉得在文本编辑器中不好看，可以用alt+shift+f可以格式化文本使得更加方便看

| 第一列 | 第二列 | 第三列 |
| :----- | -----: | :----: |
| 1      |      2 |   3    |
| 4      |      5 |   6    |


<table>
  <tbody>
    <tr>
      <th colspan="3"></th>
      <th scope="col">Subject</th>
      <th scope="col">Object</th>
    </tr>
    <tr>
      <th rowspan="5">单数</th>
      <th colspan="2">第一人称</th>
      <td>I</td>
      <td>me</td>
    </tr>
    <tr>
      <th colspan="2">第二人称</th>
      <td>you</td>
      <td>you</td>
    </tr>
    <tr>
      <th rowspan="3">第三人称</th>
      <th >♂</th>
      <td>he</td>
      <td>him</td>
    </tr>
    <tr>
      <th>♀</th>
      <td>she</td>
      <td>her</td>
    </tr>
    <tr>
      <th>o</th>
      <td>it</td>
      <td>it</td>
    </tr>
    <tr>
      <th rowspan="3" >复数</th>
      <th colspan="2" >第一人称</th>
      <td>we</td>
      <td>us</td>
    </tr>
    <tr>
      <th colspan="2">第二人称</th>
      <td>you</td>
      <td>you</td>
    </tr>
    <tr>
      <th colspan="2" >第三人称</th>
      <td>they</td>
      <td>them</td>
    </tr>
  </tbody>
</table>

h5表格的结构
```html
<table>
  <tbody>
    <!-- 标题 -->
    <tr>
      <th></th>
      <th></th>
    </tr>
    <!-- 内容主题 -->
    <tr>
        <!-- 跨列 -->
      <td colspan="2"></td>
        <!-- 跨行 -->
      <td rowspan="3"></td>
    </tr>
  </tbody>
</table>
```

## 超链接
用[ ]+()，中括号里面是需要标记的文字，小括号里面是[链接](https://www.baidu.com/home)

## 代码块，还支持高亮

用三个\`+换行+内容+换行+三个\`就可以写下代码，如果需要代码高亮显示需要在第一个三个小撇号后面写下代码名字，如： 

```c++
int num=10;
return 0;
```
## 隐藏
使用html可以实现点击打开代码块

    <details>
      <summary>点击时的区域标题</summary>
    
    ```bash
    echo "hello shell"  
    echo "hello python"  
    ```
    </details>
效果如下
<details>
  <summary>点击时的区域标题</summary>

```bash
echo "hello shell"  
echo "hello python"  
```
</details>

## 注释

与h5的注释一样，用<！-- + 内-容 + -->

## 其他mark文档学习

[markdown源码github](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet#lines)

[vscode中markdown插件网站](https://marketplace.visualstudio.com/items?itemName=yzhang.markdown-all-in-one)

<div style="background:red">name</div>

## GFM 语法

    GFM（GitHub Flavored Markdown）语法是除了标准的Markdown语法外最被广泛使用的另一种版本Markdown语法，它基于CommonMark。GFM语法与标准的Markdown语法大同小异，扩展了一些标准的Markdown语法没有的内容
 GFM与标准的Markdown标记语法的异同  
GFM语法与标准的Markdown标记语法大部分内容是相同的，这里以本系列教程前面讲过的「标准的Markdown标记语法」作为参考，给出GFM语法与标准Markdown语法的区别。

### 删除线
在标准的Markdown中是不支持「删除线」效果的标记。

在GFM中，可以通过~~实现删除线的效果：

~~我是被删除的内容~~

### 任务表
使用`- [ ] 任务1`或者`- [X] 任务2`表示一个任务

- [ ] 任务1
- [X] 任务2
  
### Emoji标签兼容(vscode不支持)

:peach:  
:EMOJICODE

[表情链接1](https://github.com/ikatyang/emoji-cheat-sheet/blob/master/README.md)    

[表情链接2](https://www.webfx.com/tools/emoji-cheat-sheet/)