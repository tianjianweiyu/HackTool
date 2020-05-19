## 前言

仿照鬼手56大佬而写

## 界面

用MFC写的  界面有点low 不要在意这些细节。

![image-20200519100257414](../AppData/Roaming/Typora/typora-user-images/image-20200519100257414.png)

## 代码视图

代码的话放一个大概的类视图 详细的就不贴了实在是太多。

![image-20200519100445573](../AppData/Roaming/Typora/typora-user-images/image-20200519100445573.png)

## 功能介绍

下面一一介绍每一个功能及其实现原理。

### 基础技术

![image-20200519100509290](../AppData/Roaming/Typora/typora-user-images/image-20200519100509290.png)

#### 防双开

使用最基础的互斥体防双开 ，默认是开启的，点击可以切换状态。

#### 释放资源

使用`FindResource``SizeofResource``LoadResource`和`LockResource`等API实现资源的释放。

程序会在同级目录下释放后续功能测试所需的DLL与exe。

### 注入技术

![image-20200519100752538](../AppData/Roaming/Typora/typora-user-images/image-20200519100752538.png)

#### 全局钩子注入

使用Windows提供的`SetWindowsHookEx`将DLL注入到系统。测试DLL为`MyHookDll.dll`。可通过资源释放技术释放所需dll。也可自行编译。HOOK的消息是`WH_GETMESSAGE`。dll中只写了一个象征性的`MessageBox`。即拖拽dll点击开始注入后，当系统中有任意进程接收到消息会弹出一个MessageBox。如果电脑中启动的应用较多可能导致蓝屏卡死等情况。

#### 远程线程注入

1. 拖拽RemoteThreadDll.dll(使用资源释放技术释放)或者自行编译
2. 输入需要注入的进程ID
3. 点击开始注入后会将测试DLL注入到目标进程 测试DLL只有一个MessageBox
4. 如果注入的进程权限较高或者检测较严格(如QQ.exe) 则可能失败

**实现原理**

使用`CreateThreadEx`函数创建远程线程 实现DLL注入。也可以修改源码使用更底层的`ZwCreateThreadEx`。

#### APC注入

1. 拖拽RemoteThreadDll.dll(使用资源释放技术释放)或自行编译
2. 输入需要注入的进程名
3. 点击开始注入后会将测试DLL注入到目标进程 测试DLL只有一个MessageBox
4. 如果注入的进程权限较高或者检测较严格(如QQ.exe) 则可能失败

**实现原理**

通过`QueueUserAPC`把一个函数添加到指定线程的`APC`队列中

### 启动技术

![image-20200519101529099](../AppData/Roaming/Typora/typora-user-images/image-20200519101529099.png)

#### 三种方式创建进程

1. 任意拖拽一个exe到窗口，点击按钮后会使用三种不同的方式创建进程
2. 分别使用`CreateProcess``ShellExecute`和`WinExec`三种方式创建进程

#### 内存加载运行dll

1. 下拉框选中dll文件
2. 点击按钮后程序会将dll资源读到内存模拟PE加载器加载到本程序内存后直接运行

### 自启动技术

![image-20200519102216475](../AppData/Roaming/Typora/typora-user-images/image-20200519102216475.png)

#### 注册表

1. 拖拽任意exe到窗口
2. 点击按钮后会将程序的完整路径写到注册表启动项
3. 目标程序随系统启动而启动
4. 可自行查看提示窗口的注册表路径进行查看或删除
5. 原理：通过`RegOpenKeyEx`和`RegSetValueEx`将程序写入到`\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run`

#### 快速启动目录

1. 拖拽任意exe到窗口
2. 点击按钮后会创建程序的快捷方式到系统的快速启动目录
3. 目标程序随用户登录而自行启动
4. 可自行查看提示窗口的快速启动目录路径进行查看或删除
5. 实现原理：使用`SHGetSpecialFolderPath`函数获取windows系统中快速启动目录的路径 然后，使用`IPersistFile`类，将想要自启动的程序的快捷方式复制到快速启动目录下

#### 计划任务

1. 此功能需使用管理员运行
2. 拖拽任意exe到窗口
3. 点击按钮后会将目标程序写入到系统的计划任务
4. 程序随用户登录而启动
5. 可自行在计划任务中删除添加的事件
6. 实现原理：调用COM组件将程序添加到计划任务实现开机自启动

#### 系统服务

1. 此功能需使用管理员运行
2. 拖拽ServiceTest.exe(使用资源释放技术释放)到窗口
3. 点击按钮后会将ServiceExeTest.exe注册为系统服务
4. ServiceTest.exe将会在后台运行
5. ServiceTest.exe不含任何代码 只是一个空的服务应用程序
6. 可自行使用任务管理器删除
7. 实现原理：使用`CreateService`将编写的服务应用程序注册为系统服务实现开机自启动

### 提权技术

![image-20200519104155255](../AppData/Roaming/Typora/typora-user-images/image-20200519104155255.png)

#### 提升为Debug权限

1. 此功能需使用管理员运行
2. 点击按钮后程序将当前进程提升为为Debug权限

### BypassUAC

1. 首先将用户账户控制设置为默认
2. 点击按钮后会绕过UAC的弹窗提升为管理员权限
3. 不弹窗开启一个管理员权限的命令行
4. 实现原理：使用COM组件提权

### 隐藏技术

![image-20200519104301022](../AppData/Roaming/Typora/typora-user-images/image-20200519104301022.png)

#### 进程伪装

1. 任意拖入exe文件到窗口
2. 点击按钮后会将本进程伪装为拖入的exe文件
3. 本程序为32位 在32为系统下此功能才有效
4. 实现原理：使用`NtQueryInformationProcess`获取进程环境块。修改指定进程环境块的进程路径以及命令行信息，从而达到进程伪装的效果。

#### 傀儡进程

1. 拖拽任意一个exe到窗口
2. 点击按钮后会将一段MessageBox的ShellCode注入到目标exe
3. 实现原理：修改某一进程的内存数据，向内存数据写入Shellcode代码，并修改该进程的执行流程，使其执行Shellcode代码。这样，进程还是原来的进程，但执行的操作却替换了。

#### 隐藏进程

1. 拖拽`HIdeProcessDll.dll`(使用资源释放技术释放)或者自行编译
2. 点击按钮后会将dll注入到任务管理器，实现进程隐藏
3. 实现原理：通过InlineHook函数`ZwQuerySystemInformation`，遍历进程时不返回自己的进程

#### DLL劫持

1. 此功能没有写在界面上 需要自行手动操作
2. 我这里劫持的是`ServiceTest.exe`的`vcruntime140d.dll`
3. 找到`HijackDll.dll`(使用资源释放技术释放)或自行编译
4. 将DLL重命名为`vcruntime140d.dll`
5. 找到C:\Windows\SysWOW64目录下的`vcruntime140d.dll`将其重命名为`vcruntime140dOrg.dll`(如果无法重命名请自行百度解决方案)
6. 将`vcruntime140d.dll`放到SysWOW64目录下
7. 运行`ServiceTest.exe`
8. 劫持的DLL在DllMian中写了一个MessageBox函数 表示劫持成功
9. 实现原理：在PE加载器加载DLL时 使用自制的DLL替换掉原DLL ，在自制的DLL中转发原DLL的导出函数并添加代码

### 压缩技术

![image-20200519105922541](../AppData/Roaming/Typora/typora-user-images/image-20200519105922541.png)

#### 数据解压缩

1. 在文本框中输入数据 点击按钮，即可对输入的数据压缩和解压缩
2. 实现原理：通过Windows提供的`RtlDecompressBuffer`和`RtlGetCompressionWorkSpaceSize`实现对数据的解压缩

#### 文件解压缩

1. 拖拽文件到窗口，点击相应按钮 即可对文件进行压缩和解压缩
2. 使用`ZLIB`压缩库实现对文件的解压缩
3. 压缩会在同级目录下生成个`.XiaoBai`后缀的文件，名称还是原文件名
4. 解压缩需拖入`.XiaoBai`后缀的文件，生成一个新文件，格式为`原文件名_XB.exe`

### 加密技术

![image-20200519110417306](../AppData/Roaming/Typora/typora-user-images/image-20200519110417306.png)

#### HASH

拖拽文件到窗口，点击按钮 即可求得文件的哈希值

#### AES加/解密

在数据窗口输入数据  编辑框输入AES密钥 点击按钮 即可求得数据经过AES加解密后的值

#### RSA加/解密

在数据窗口输入数据 点击按钮 即可求得数据经过RAS加解密后的值

#### 实现原理

以上加解密都是使用Windows自带的加密库来实现的，使用的是`cryptoAPI`。

### 功能技术

![image-20200519110626252](../AppData/Roaming/Typora/typora-user-images/image-20200519110626252.png)

#### 进程遍历

对当前所有进程进行遍历

实现原理：使用`CreateToolhelp32Snapshot`、`Process32First`和`Process32Next`

#### 文件遍历

1. 对当前系统所有文件进行遍历 可查看文件属性和磁盘信息
2. 可查看文件属性和磁盘信息 双击可进入下一层目录
3. 实现原理：使用`FindFirstFile`和`FindNextFile`

#### 桌面截屏

1. 对当前屏幕进行截屏 截图存放在程序同级目录
2. 实现原理：通过GDI的方式实现屏幕画面抓取

#### 按键记录

1. 实现了一个键盘记录器 记录下所有的按键记录
2. 按键记录存储在同级目录
3. 实现原理：利用原始输入模型。

#### 显示CMD结果

1. 在命令框输入CMD命令 例如ping 127.0.0.0
2. 等命令执行完成会在数据窗口会显示CMD命令的执行结果
3. 由于环境不一致 这个功能可能会执行失败
4. 实现原理：使用匿名管道进行进程通讯，从而获得cmd的执行结果

#### U盘小偷

1. 在功能窗口此功能默认开启
2. 对U盘的插入和拔出进行监控
3. 拷贝U盘里所有的.txt文件到程序的当前目录下,如需拷贝其他文件则修改源码
4. 实现原理:响应`WM_DEVICECHANGE`监控U盘，获取盘符之后进行文件遍历 即可拷贝所需要的文件

#### 文件监控

1. 对选定的目录上的文件操作进行监控
2. 实现原理：Windows提供了一个文件监控的接口函数ReadDirectoryChangesW，这个监控函数可以对计算机上的所有文件操作进行监控。

#### 自删除

1. 点击后程序将退出进程 并删除同级目录下所有exe文件
2. 实现原理：使用批处理
3. 慎用！！！

## 说明

1. 本程序仅用于交流和研究，请勿用于其他用途。有任何后果与本人无关
2. 由于环境不一样 所以不能保证每个功能在每台机器上都是可行的。如果程序有bug还请多多体谅
3. 如果发现缺dll或者没有测试文件的情况在工程项目下都可找到，可自行编译。