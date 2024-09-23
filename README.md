# Light

这是一款游戏引擎（正在开发中）。

其名 Light 的含义描述了它的具备的三个特征：

1. 光照：这是一款支持图形渲染的引擎。
2. 轻量：这是一款结构简单易用的引擎。
3. 启迪：这是一款用于自我学习的引擎。

## 项目结构

1. Light 是一个由多个静态库构成的游戏引擎，开发者通过接入这些静态库并编译来运行引擎。
2. 引擎代码高度模块化，上述的每个静态库都是一个模块，并利用 cmake 实现了类似包管理的机制。
3. 这些模块都存在 sources 文件夹中，开发者可以单独引用或附加引用这些模块，来自行实现或扩展功能。
4. 针对每个模块，通常都具有以下内容结构：
   - Runtime：引擎核心代码，用户运行时必备。
   - Editor：基于 Runtime 开发编辑器功能，便于开发者使用。
   - Tests：用于检测模块问题的单元测试。
5. 查看 samples 文件夹中的示例项目，获知如何正确使用该引擎。

## 环境依赖

- cmake：构建系统
- vcpkg：包管理器

## 为什么我需要 Light？

1. 我需要一个便于进行引擎底层运行原理研究的游戏引擎：

   虽然游戏引擎让游戏开发的门槛变得非常低，但当要进行一些特殊功能的开发缺乏底层知识是万万不行的，但很多教程对此都是一笔带过，这对我的进一步学习产生了阻碍，所以我必须得想办法搞懂这些。

2. 我想要一个真正源码随时可见随时可改的引擎：

   大部分引擎都是语言上的套壳引擎，C++ 写核心再用 C# 套一层，导致看源码很不方便也缺少反馈性。有说法是 C# 更简单易学所以使用 C# 套壳，但我认为只要不去深究 C++ 的复杂功能，C++ 也足够简单，而且很多学校也教。

3. 我想要一个足够简单易于扩展的引擎：

   市面上成熟的引擎都太复杂了，而我想要的引擎，它可能不支持先进的功能也没有强大的运行性能，但它一定足够简单直白，易于理解和修改它。甚至它可以不是一款游戏引擎，而是简化到成为了一种框架库，这样我才能更自由的用它发挥畅想。

4. 我想要一个开源引擎：

   闭源引擎无法知晓内部运行机制，出了问题也没法修复，而且在它们的基础上写代码会给我一种给人白打工还助其垄断的感觉。

## Light 将发展到哪一步？

1. Light 不仅是游戏引擎，Light 将成为我长久发展的研究基地：

   Light 的基本功能是一款可用于制作简单游戏的引擎，因为它诞生的一个目的就是为了让我研究游戏引擎的运行原理。但同时它也将成为我的个人 C++ 工具库、研究基地，这也是为什么必须将它高度模块化。未来如果有任何我感兴趣且需要 C++功能实现，我大概率都会以 Light 为基础开发。

2. Light 目前仅用于学习，故仅会制造有意义的轮子：

   时间是有限的，技术是垄断的，所以开发 Light 大概率只是生活的消遣而非生存的方式。开发 Light 是为了更好的使用其他商业引擎，故不能在上面耗费太多的精力，我只会造那些自己感兴趣有学习需要的轮子。因此 Light 将始终保持学生作业的水准，但这也是没有办法的事。
