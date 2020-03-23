// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

//共享内存
#pragma data_seg("mydata")
	HHOOK g_hHook;			//模块句柄
#pragma data_seg()
#pragma comment(linker,"/SECTION:mydata,RWS")

HMODULE g_hModule;		//钩子句柄	

extern "C" _declspec(dllexport)
//卸载钩子
BOOL UnsetGlobalHook()
{
	if (g_hHook)
	{
		UnhookWindowsHookEx(g_hHook);
	}
	return TRUE;
}

//钩子的回调函数
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	MessageBox(NULL, L"你被监视了", L"提示", MB_ICONWARNING | MB_OKCANCEL);

	//CallNextHookEx函数表示将当前钩子传递给钩子链中的下一个钩子
	//第一个参数当前钩子的句柄。如果直接返回0，则表示中断钩子传递
	//对钩子进行拦截
	return CallNextHookEx(g_hHook, code, wParam, lParam);


}

extern "C" _declspec(dllexport)
//设置全局钩子
BOOL SetGlobalHook()
{
	//SetWindowsHookEx，第一个参数表示钩子的类型，WH_GETMESSAGE表示安装消息队列的消息钩子
	//可以监视发送到消息队列的消息。第二个参数表示钩子回调函数，第三个参数表示包含钩子回调
	//函数的DLL模块句柄，如果要设置全局钩子，则该参数必须指定DLL模块句柄，第四个参数表示与
	//钩子关联的线程ID，0表示为全局钩子，关联所有线程。
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hModule, 0);
	if (g_hHook == NULL)
	{
		return FALSE;
	}
	return TRUE;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		g_hModule = hModule;
		break;
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

