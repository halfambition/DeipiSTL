# DeipiSTL

## DeipiSTL是我的C++练习项目，采用c++11语法，根据SGI STL源码，编写的简易STL库。

## 项目受到[TinySTL开源项目](https://github.com/zouxiaohang/TinySTL/tree/master)的启发。这会是一个非常棒的，练习模版编程、lambda等，熟悉C++11新特性的项目，同时从SGI STL源码中学习许多代码编写技巧。

## 我会在下方继续更新我的项目编写完成进度。分享我的学习经验，遇到的问题以及解决方案。同时会更新我阅读源码时对我有很大帮助的博客。

### 项目IDE：Visual Studio 2019、Xcode 14.3.1

### 项目github地址：https://github.com/halfambition/DeipiSTL

### 项目计划：

六大组件：

- [x]  分配器
- [x]  迭代器
- [ ]  容器
- [ ]  算法
- [ ]  仿函数
- [ ]  适配器

其他新增内容：

- [ ]  Skip List
- [ ]  Mat
- [ ]  Bloom Fliter
- [ ]  B-Tree
- [ ]  红黑树

### 遇到的问题及解决方案：

本项目主要跟随侯捷大佬的STL源码剖析一书进行

同时分享一个十分方便的[github链接](https://github.com/arkingc/note/blob/master/C++/STL%E6%BA%90%E7%A0%81%E5%89%96%E6%9E%90.md#1%E7%A9%BA%E9%97%B4%E5%88%86%E9%85%8D%E5%99%A8%E7%9A%84%E6%A0%87%E5%87%86%E6%8E%A5%E5%8F%A3)，其将SYL源码剖析markdown化，并且在许多地方会有直接跳到源码的链接（虽然我认为源码中缭乱的宏定义使得源码可读性极差，我个人功力也确实差劲）。

**分配器部分：**

编写分配器时，最主要的问题出在二级分配器上。对于二级分配器中的refill和chunk_alloc两个方法，我是全然没有头绪的。虽然书中给出了chunk_alloc的运行逻辑，不过好在发现[这样一篇博客](https://www.cnblogs.com/grass-and-moon/p/13690272.html)，里面实现了一个简易的二级分配器，对我帮助很大。

经验分享：

如何寻找距离a最近的且大于a的8的倍数：(a+8-1)&~(8-1)

对于源码中寻找list索引的函数为((x+8-1)/8) -1；不能替换成x/8，因为我们希望x=8时，对应的索引仍是0。

关于如何设置new-handler，在[effectiveC++](https://zhuanlan.zhihu.com/p/621194847)中有详细叙述。

关于二级分配器中的deallcate中，我最初设计的是释放的内存先进入内存池mem_block中，后来发现返回的内存不一定是8的倍数，因此可能会比8小然后造成union指针失效。

[C++ STL 源码剖析之 Traits 编程技法 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/85809752)

traits编程技法让我受益良多，才知道还能这么搞
