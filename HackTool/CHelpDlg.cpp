// CHelpDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CHelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg 对话框

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HELP_DIALOG, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
END_MESSAGE_MAP()


// CHelpDlg 消息处理程序


BOOL CHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	HTREEITEM Basic = m_Tree.InsertItem(L"基础技术");

	HTREEITEM ReleaseResoures = m_Tree.InsertItem(L"资源释放", Basic);
	HTREEITEM ReleaseResouresHelp = m_Tree.InsertItem(L"在同级目录下释放后续功能测试所需的DLL", ReleaseResoures);

	HTREEITEM RunSigle = m_Tree.InsertItem(L"防多开",Basic);
	HTREEITEM RunSigleHelp = m_Tree.InsertItem(L"使用的是互斥体防多开 默认开启 点击防多开按钮可以开启与关闭防多开机制",RunSigle);

	HTREEITEM Inject = m_Tree.InsertItem(L"注入技术");

	HTREEITEM GlobalHook = m_Tree.InsertItem(L"全局钩子注入", Inject);
	HTREEITEM GlobalHookHelp1 = m_Tree.InsertItem(L"需拖拽MyHookDll.dll(使用资源释放技术释放)到窗口 测试DLL只有一个MessageBox", GlobalHook);
	HTREEITEM GlobalHookHelp2 = m_Tree.InsertItem(L"HOOK的是WM_GETMESSAGE消息 只要系统中有程序接收消息就会弹框", GlobalHook);
	HTREEITEM GlobalHookHelp3 = m_Tree.InsertItem(L"如果运行程序开启较多 可能直接卡死 慎用 慎用", GlobalHook);

	HTREEITEM RemoteThread = m_Tree.InsertItem(L"远程线程注入", Inject);
	HTREEITEM RemoteThreadHelp1 = m_Tree.InsertItem(L"1.拖拽RemoteThreadDll.dll(使用资源释放技术释放)", RemoteThread);
	HTREEITEM RemoteThreadHelp2 = m_Tree.InsertItem(L"2.输入需要注入的进程ID", RemoteThread);
	HTREEITEM RemoteThreadHelp3 = m_Tree.InsertItem(L"3.点击开始注入后会将测试DLL注入到目标进程 测试DLL只有一个MessageBox", RemoteThread);
	HTREEITEM RemoteThreadHelp4 = m_Tree.InsertItem(L"4.如果注入的进程权限较高或者检测较严格(如QQ.exe) 则可能失败", RemoteThread);

	HTREEITEM APCInject = m_Tree.InsertItem(L"APC注入", Inject);
	HTREEITEM APCInjectHelp1 = m_Tree.InsertItem(L"1.拖拽RemoteThreadDll.dll(使用资源释放技术释放)", APCInject);
	HTREEITEM APCInjectHelp2 = m_Tree.InsertItem(L"2.输入需要注入的进程名", APCInject);
	HTREEITEM APCInjectHelp3 = m_Tree.InsertItem(L"3.点击开始注入后会将测试DLL注入到目标进程 测试DLL只有一个MessageBox", APCInject);
	HTREEITEM APCInjectHelp4 = m_Tree.InsertItem(L"4.如果注入的进程权限较高或者检测较严格(如QQ.exe) 则可能失败", APCInject);

	HTREEITEM Start = m_Tree.InsertItem(L"启动技术");

	HTREEITEM Create_Process = m_Tree.InsertItem(L"创建进程API", Start);
	HTREEITEM Create_ProcessHelp1 = m_Tree.InsertItem(L"任意拖拽一个exe到窗口", Create_Process);
	HTREEITEM Create_ProcessHelp2 = m_Tree.InsertItem(L"选中一种创建方式", Create_Process);
	HTREEITEM Create_ProcessHelp3 = m_Tree.InsertItem(L"点击创建进程按钮后会使用选定的方式创建进程", Create_Process);


	HTREEITEM MemoryLoad = m_Tree.InsertItem(L"内存加载运行", Start);
	HTREEITEM MemoryLoadHelp1 = m_Tree.InsertItem(L"下拉框选中dll文件(dll文件作为资源插入到了程序中)", MemoryLoad);
	HTREEITEM MemoryLoadHelp2 = m_Tree.InsertItem(L"点击内存直接加载运行按钮后程序会将dll资源加载到本程序内存后直接运行", MemoryLoad);
	HTREEITEM MemoryLoadHelp3 = m_Tree.InsertItem(L"测试DLL只有一个MessageBox", MemoryLoad);
	HTREEITEM MemoryLoadHelp4 = m_Tree.InsertItem(L"exe文件有问题，待解决", MemoryLoad);


	HTREEITEM SelfStart = m_Tree.InsertItem(L"自启动技术");

	HTREEITEM RegEdit = m_Tree.InsertItem(L"注册表", SelfStart);
	HTREEITEM RegEditHelp1 = m_Tree.InsertItem(L"拖拽任意exe到窗口", RegEdit);
	HTREEITEM RegEditHelp2 = m_Tree.InsertItem(L"点击按钮后会将程序的完整路径写到注册表启动项", RegEdit);
	HTREEITEM RegEditHelp3 = m_Tree.InsertItem(L"目标程序随系统启动而启动", RegEdit);
	HTREEITEM RegEditHelp4 = m_Tree.InsertItem(L" 可自行查看提示窗口的注册表路径进行查看或删除", RegEdit);

	HTREEITEM StartDir = m_Tree.InsertItem(L"快速启动目录", SelfStart);
	HTREEITEM StartDirHelp1 = m_Tree.InsertItem(L"拖拽任意exe到窗口", StartDir);
	HTREEITEM StartDirHelp2 = m_Tree.InsertItem(L"点击按钮后会将程序的快捷方式生成到系统的快速启动目录", StartDir);
	HTREEITEM StartDirHelp3 = m_Tree.InsertItem(L"目标程序随用户登录而自行启动", StartDir);
	HTREEITEM StartDirHelp4 = m_Tree.InsertItem(L" 可自行查看提示窗口的快速启动目录路径进行查看或删除", StartDir);

	HTREEITEM Task = m_Tree.InsertItem(L"计划任务", SelfStart);
	HTREEITEM TaskHelp1 = m_Tree.InsertItem(L"此功能需使用管理员运行", Task);
	HTREEITEM TaskHelp2 = m_Tree.InsertItem(L"拖拽任意exe到窗口", Task);
	HTREEITEM TaskHelp3 = m_Tree.InsertItem(L"点击按钮后会将目标程序写入到系统的计划任务", Task);
	HTREEITEM TaskHelp4 = m_Tree.InsertItem(L"程序随用户登录而启动", Task);
	HTREEITEM TaskHelp5 = m_Tree.InsertItem(L"可自行在计划任务中删除添加的事件", Task);

	HTREEITEM SystemService = m_Tree.InsertItem(L"系统服务", SelfStart);
	HTREEITEM SystemServiceHelp1 = m_Tree.InsertItem(L"此功能需使用管理员运行", SystemService);
	HTREEITEM SystemServiceHelp2 = m_Tree.InsertItem(L"拖拽ServiceTest.exe到窗口(使用资源释放技术释放)", SystemService);
	HTREEITEM SystemServiceHelp3 = m_Tree.InsertItem(L"点击按钮后会将ServiceTest.exe注册为系统服务", SystemService);
	HTREEITEM SystemServiceHelp4 = m_Tree.InsertItem(L"ServiceTest.exe将会在后台运行,重启会自启动", SystemService);
	HTREEITEM SystemServiceHelp5 = m_Tree.InsertItem(L"ServiceTest.exe不含任何代码 只是一个空的服务应用程序", SystemService);
	HTREEITEM SystemServiceHelp6 = m_Tree.InsertItem(L"可自行使用任务管理器删除", SystemService);


	HTREEITEM Privilege = m_Tree.InsertItem(L"提权技术");

	HTREEITEM Debug = m_Tree.InsertItem(L"提升为Debug权限", Privilege);
	HTREEITEM DebugHelp1 = m_Tree.InsertItem(L"此功能需使用管理员运行", Debug);
	HTREEITEM DebugHelp2 = m_Tree.InsertItem(L"点击按钮后程序将获取Debug权限", Debug);

	HTREEITEM BypassUAC = m_Tree.InsertItem(L"BypassUAC", Privilege);
	HTREEITEM BypassUACHelp1 = m_Tree.InsertItem(L"点击按钮后会使用COM组件提升为管理员权限", BypassUAC);
	HTREEITEM BypassUACHelp2 = m_Tree.InsertItem(L"不弹出用户账户控制", BypassUAC);
	HTREEITEM BypassUACHelp3 = m_Tree.InsertItem(L"会先弹出一个MessageBox，因为与之前共用一个dll，然后开启一个管理员权限的命令行", BypassUAC); 

	HTREEITEM Hide = m_Tree.InsertItem(L"隐藏技术");

	HTREEITEM Pretend = m_Tree.InsertItem(L"进程伪装", Hide);
	HTREEITEM PretendHelp1 = m_Tree.InsertItem(L"拖拽任意exe到窗口", Pretend);
	HTREEITEM PretendHelp2 = m_Tree.InsertItem(L"点击按钮后会将本进程伪装为拖拽的文件", Pretend);
	HTREEITEM PretendHelp3 = m_Tree.InsertItem(L"使用ProcessExplorer.exe查看才能看到效果", Pretend);
	HTREEITEM PretendHelp4 = m_Tree.InsertItem(L"本程序为32位 在32为系统下此功能才有效", Pretend);

	HTREEITEM Puppet = m_Tree.InsertItem(L"傀儡进程", Hide);
	HTREEITEM PuppetHelp1 = m_Tree.InsertItem(L"拖拽任意一个exe到窗口", Puppet);
	HTREEITEM PuppetHelp2 = m_Tree.InsertItem(L"点击按钮后会将一段MessageBox的ShellCode注入到目标exe", Puppet);

	HTREEITEM HideProcess = m_Tree.InsertItem(L"隐藏进程", Hide);
	HTREEITEM HideProcess1 = m_Tree.InsertItem(L"需要先释放资源", HideProcess);
	HTREEITEM HideProcess2 = m_Tree.InsertItem(L"直接点击按钮后会将HIdeProcessDll.dll注入任务管理器，实现进程隐藏", HideProcess);

	HTREEITEM DllHijack = m_Tree.InsertItem(L"DLL劫持", Hide);
	HTREEITEM DllHijackHelp1 = m_Tree.InsertItem(L"原理：在PE加载器加载DLL时 使用自制的DLL替换掉原DLL 并在自制的DLL中添加代码", DllHijack);
	HTREEITEM DllHijackHelp2 = m_Tree.InsertItem(L"此功能没有写在界面上 可自行手动操作", DllHijack);
	HTREEITEM DllHijackHelp3 = m_Tree.InsertItem(L"1.找到HijackDll.dll(使用资源释放技术释放)", DllHijack);
	HTREEITEM DllHijackHelp4 = m_Tree.InsertItem(L"2.将DLL重命名为vcruntime140d.dll", DllHijack);
	HTREEITEM DllHijackHelp5 = m_Tree.InsertItem(L"3.找到C:\\Windows\\SysWOW64目录下的vcruntime140d.dll将其重命名为vcruntime140dOrg)", DllHijack);
	HTREEITEM DllHijackHelp6 = m_Tree.InsertItem(L"4.如果无法重命名请自行百度解决方案", DllHijack);
	HTREEITEM DllHijackHelp7 = m_Tree.InsertItem(L"5.将vcruntime140d.dll放到SysWOW64目录下", DllHijack);
	HTREEITEM DllHijackHelp8 = m_Tree.InsertItem(L"6.运行ServiceTest.exe", DllHijack);
	HTREEITEM DllHijackHelp9 = m_Tree.InsertItem(L"7.劫持的DLL转发了原DLL的所有函数 不会对程序运行造成影响", DllHijack);
	HTREEITEM DllHijackHelp10 = m_Tree.InsertItem(L"8.劫持的DLL在DllMian中写了一个MessageBox函数 表示劫持成功", DllHijack);

	HTREEITEM Compress = m_Tree.InsertItem(L"压缩技术");

	HTREEITEM DataCompress = m_Tree.InsertItem(L"数据压缩", Compress);
	HTREEITEM DataCompressHelp1 = m_Tree.InsertItem(L"在文本框中输入数据 点击按钮，即可对输入的数据压缩和解压缩", DataCompress);

	HTREEITEM FileCompress = m_Tree.InsertItem(L"文件压缩", Compress);
	HTREEITEM FileCompressHelp1 = m_Tree.InsertItem(L"拖拽文件到窗口，点击按钮 即可对文件进行压缩并生成压缩文件", FileCompress);
	HTREEITEM FileCompressHelp2 = m_Tree.InsertItem(L"压缩文件为原文件名+XiaoBai后缀", FileCompress);

	HTREEITEM FileUnCompress = m_Tree.InsertItem(L"文件解压缩", Compress);
	HTREEITEM FileUnCompressHelp1 = m_Tree.InsertItem(L"拖拽XiaoBai后缀的压缩文件到窗口，点击按钮 即可对文件进行解压缩并生成解压缩文件", FileUnCompress);
	HTREEITEM FileUnCompressHelp2 = m_Tree.InsertItem(L"解压缩文件为原文件名_XB+exe后缀", FileUnCompress);

	HTREEITEM Encrypt = m_Tree.InsertItem(L"加密技术");

	HTREEITEM HASH = m_Tree.InsertItem(L"HASH", Encrypt);
	HTREEITEM HASHHelp1 = m_Tree.InsertItem(L"拖拽文件到窗口，点击按钮 即可求得文件的哈希值", HASH);


	HTREEITEM AES = m_Tree.InsertItem(L"AES加/解密", Encrypt);
	HTREEITEM AESHelp1 = m_Tree.InsertItem(L"在AES编辑框输入密钥", AES);
	HTREEITEM AESHelp2 = m_Tree.InsertItem(L"在数据窗口输入数据 点击按钮 即可求得数据经过AES加解密后的值", AES);

	HTREEITEM RAS = m_Tree.InsertItem(L"RAS加/解密", Encrypt);
	HTREEITEM RASHelp1 = m_Tree.InsertItem(L"在数据窗口输入数据 点击按钮 即可求得数据经过RAS加解密后的值", RAS);

	HTREEITEM Function = m_Tree.InsertItem(L"功能技术");

	HTREEITEM FindAllProcess = m_Tree.InsertItem(L"进程遍历", Function);
	HTREEITEM FindAllProcessHelp1 = m_Tree.InsertItem(L"对当前所有进程进行遍历", FindAllProcess);

	HTREEITEM FindAllFile = m_Tree.InsertItem(L"文件遍历", Function);
	HTREEITEM FindAllFileHelp1 = m_Tree.InsertItem(L"默认对C盘所有文件进行遍历 可查看文件属性和磁盘信息", FindAllFile);
	HTREEITEM FindAllFileHelp2 = m_Tree.InsertItem(L"可查看文件属性和磁盘信息 双击可进入下一层目录", FindAllFile);

	HTREEITEM ScreenShot = m_Tree.InsertItem(L"桌面截屏", Function);
	HTREEITEM ScreenShotHelp1 = m_Tree.InsertItem(L"对当前屏幕进行截屏 截图存放在程序同级目录下，名为mybmp1.jpg", ScreenShot);


	HTREEITEM KeyBoardRecord = m_Tree.InsertItem(L"按键记录", Function);
	HTREEITEM KeyBoardRecordHelp1 = m_Tree.InsertItem(L"实现了一个键盘记录器 记录下所有的按键记录", KeyBoardRecord);
	HTREEITEM KeyBoardRecordHelp2 = m_Tree.InsertItem(L"按键记录存储在程序同级目录下，名为keylog.txt", KeyBoardRecord);

	HTREEITEM RemoteCMD = m_Tree.InsertItem(L"远程CMD", Function);
	HTREEITEM RemoteCMDHelp1 = m_Tree.InsertItem(L"在命令框输入CMD命令 例如ping 127.0.0.0", RemoteCMD);
	HTREEITEM RemoteCMDHelp2 = m_Tree.InsertItem(L"在数据窗口会显示CMD命令的执行结果", RemoteCMD);
	HTREEITEM RemoteCMDHelp3 = m_Tree.InsertItem(L"没有添加远程", RemoteCMD);

	HTREEITEM USBMonitoring = m_Tree.InsertItem(L"U盘小偷", Function);
	HTREEITEM USBMonitoringHelp1 = m_Tree.InsertItem(L"1.对U盘的插入和拔出进行监控 默认开启", USBMonitoring);
	HTREEITEM USBMonitoringHelp2 = m_Tree.InsertItem(L"2.拷贝U盘里所有的.txt文件到程序的当前目录下,如需拷贝其他文件则修改源码", USBMonitoring);

	HTREEITEM FileMonitoring = m_Tree.InsertItem(L"文件监控", Function);
	HTREEITEM FileMonitoringHelp1 = m_Tree.InsertItem(L"拖入要监视的目录", FileMonitoring);
	HTREEITEM FileMonitoringHelp2 = m_Tree.InsertItem(L"点击开始监控按钮会对监视的目录上的文件操作进行监控", FileMonitoring);
	HTREEITEM FileMonitoringHelp3 = m_Tree.InsertItem(L"包括新增，删除，修改，重命名操作", FileMonitoring);

	HTREEITEM DelSelf = m_Tree.InsertItem(L"自删除", Function);
	HTREEITEM DelSelfHelp1 = m_Tree.InsertItem(L"点击自删除按钮会删除程序所在目录下所有的exe文件", DelSelf);
	HTREEITEM DelSelfHelp2 = m_Tree.InsertItem(L"慎用!慎用!", DelSelf);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
