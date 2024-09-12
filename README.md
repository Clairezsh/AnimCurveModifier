# Animation Curve Modifier
Animation Curve Modifier是一个用于在UE中快速编辑AnimSequence的工具。该工具支持批量为AnimSequence新增、复制、重命名和删除动画曲线。
<h2 id="wDfV9">使用演示</h2>

+ 视频链接：https://youtu.be/0l16tB7kUg4?si=bf6Aj0nZ1PGrOSLc

+ MP4文件：见AnimCurveModifier使用演示.mp4

<h2 id="wDfV9">开发环境</h2>

+ 编程语言：C++，
+ 界面：Slate，
+ 引擎版本：UE 5.2

目前暂时**只支持 UE 5.2**，还未作其它版本的 API 适配。


<h2 id="WjGRD">文件结构</h2>
工具的源码位于 Plugins / CoreManager 下。

+ CoreManager.cpp - 插件调用的入口文件，用于加载插件，创建菜单栏选项和下拉按钮，打开工具窗口；
+ CoreManagerCommand.cpp - 定义和注册打开窗口的 UI_Command，用于给菜单栏按钮绑定命令；
+ SAnimCurveModifierWidget.cpp - 窗口的 Slate 代码文件，以及按钮的基础功能；
+ AnimCurveModifierFunctions.cpp - 处理 AnimSequence 的核心函数，包含新增、复制、重命名和删除 AnimSequence 中动画曲线的功能实现。

<h2 id="Hv9Vr">注意事项</h2>
+ 请将输入法设置成英文后，再在文本框中进行输入。
+ 若使用过程中工具异常，请重启工具后再进行尝试。
+ 若操作完之后动画数据未更新，可以关闭和重新打开动画序列来刷新更改。

<h2 id="MmHz1">如何使用</h2>
两种方法加载插件：

+ 直接将整个 UE 工程下载到本地，用 UE 打开 DevProject.uproject，在最上方菜单栏打开工具。
+ 将 DevProject / Plugins 文件夹复制到自己工程目录下使用。

加载插件后，可通过最上方菜单栏**“CustomUtils -> Animation Curve Modifier”**来打开工具窗口。

![](https://cdn.nlark.com/yuque/0/2024/png/1261680/1726050128093-9e878f0a-3717-4594-979b-9f19277700aa.png)









