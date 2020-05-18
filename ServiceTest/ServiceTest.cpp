// ServiceTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include <windows.h>
#include <wtsapi32.h>
#include <userenv.h>
#pragma comment(lib,"Wtsapi32.lib")
#pragma comment(lib,"Userenv.lib")


void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv);
void __stdcall ServiceCtrlHandle(DWORD dwOperateCode);
BOOL TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress);
void DoTask();
BOOL CreateUserProcess(char *lpszFileName);

//服务名
char g_szServiceName[MAX_PATH] = "hl15pb.exe";
SERVICE_STATUS_HANDLE g_ServiceStatusHandle = { 0 };	//服务状态句柄

// 服务入口函数以及处理回调函数
void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv)
{
	//1.调用RegisterServiceCtrlHandler函数去通知SCM它的CtrlHandler回调函数的地址
	g_ServiceStatusHandle = RegisterServiceCtrlHandlerA(g_szServiceName, ServiceCtrlHandle);

	//2.调用SetServiceStatus告诉SCM服务正在继续初始化
	TellSCM(SERVICE_START_PENDING, 0, 1);
	TellSCM(SERVICE_RUNNING, 0, 0);

	// 自定义的功能代码放在这里
	// !!注意!! 此处一定要为死循环, 否则在关机再开机的情况(不是点击重启), 不能创建用户进程
	while (TRUE)
	{
		Sleep(1000);
		DoTask();
	}
}

//主进程通知SCM在可执行文件中含有几个服务，
//并且给出每一个服务的ServiceMain回调(Call Back)函数的地址。
int main()
{
    //注册服务入口函数
	//完成ServiceMain的初始化
	//这个结构体记录了这个服务程序里面所包含的所有服务的名称和服务的入口函数
	SERVICE_TABLE_ENTRY stDispatchTable[] = { {g_szServiceName,(LPSERVICE_MAIN_FUNCTION)ServiceMain},{NULL,NULL} };

	//调用StartServiceCtrlDispatcher和SCM通讯
	//StartServiceCtrlDispatcher会为每一个传递到它的数组中的非空元素产生一个新的线程
	StartServiceCtrlDispatcher(stDispatchTable);

	return 0;
}


BOOL TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress)
{
	SERVICE_STATUS ServiceStatus = { 0 };
	BOOL bRet = FALSE;
	RtlZeroMemory(&ServiceStatus, sizeof(ServiceStatus));
	//服务可执行文件的类型
	ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	//告诉SCM你的服务的现行状态
	ServiceStatus.dwCurrentState = dwState;
	//指明服务愿意接受什么样的控制通知 允许停止服务 暂停或继续服务 关闭服务
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
	//设置退出码
	ServiceStatus.dwWin32ExitCode = dwExitCode;
	ServiceStatus.dwWaitHint = 3000;
	//设置服务运行状态
	bRet = SetServiceStatus(g_ServiceStatusHandle, &ServiceStatus);
	return bRet;
}

//服务控制函数
void __stdcall ServiceCtrlHandle(DWORD dwOperateCode)
{
	switch (dwOperateCode)
	{
	case SERVICE_CONTROL_PAUSE:
	{
		// 暂停
		TellSCM(SERVICE_PAUSE_PENDING, 0, 1);
		TellSCM(SERVICE_PAUSED, 0, 0);
		break;
	}
	case SERVICE_CONTROL_CONTINUE:
	{
		// 继续
		TellSCM(SERVICE_CONTINUE_PENDING, 0, 1);
		TellSCM(SERVICE_RUNNING, 0, 0);
		break;
	}
	case SERVICE_CONTROL_STOP:
	{
		// 停止
		TellSCM(SERVICE_STOP_PENDING, 0, 1);
		TellSCM(SERVICE_STOPPED, 0, 0);
		break;
	}
	case SERVICE_CONTROL_INTERROGATE:
	{
		// 询问
		break;
	}
	default:
		break;
	}
}

void DoTask()
{
	//要自启动程序的路径
	//char lpszFileName[MAX_PATH] = "C:\\Users\\15pb-win7\\Desktop\\HackTool.exe";
	//自己的代码写在这里
	//BOOL bRet = CreateUserProcess(lpszFileName);
	printf("这里是服务控制程序！");
	//system("pause");
}

// 突破SESSION 0隔离创建用户进程
BOOL CreateUserProcess(char *lpszFileName)
{
	HANDLE hToken;
	HANDLE hDuplicatedToken;
	LPVOID lpEnvironment;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// 获得当前Session Id
	DWORD dwSessionID = WTSGetActiveConsoleSessionId();
	// 获得当前会话的用户令牌
	if (FALSE == ::WTSQueryUserToken(dwSessionID, &hToken))
	{
		return FALSE;
	}
	// 复制令牌
	if (FALSE == DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
		SecurityIdentification, TokenPrimary, &hDuplicatedToken))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	// 创建用户会话环境
	if (FALSE == CreateEnvironmentBlock(&lpEnvironment,
		hDuplicatedToken, FALSE))
	{
		CloseHandle(hToken);
		CloseHandle(hDuplicatedToken);
		return FALSE;
	}
	// 在复制的用户会话下执行应用程序，创建进程
	if (FALSE == CreateProcessAsUser(hDuplicatedToken,
		lpszFileName, NULL, NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
		lpEnvironment, NULL, &si, &pi))
	{
		CloseHandle(hToken);
		CloseHandle(hDuplicatedToken);
		return FALSE;
	}
	
	// 关闭句柄，释放资源
	CloseHandle(hToken);
	CloseHandle(hDuplicatedToken);
	CloseHandle(pi.hProcess);
	return TRUE;
}