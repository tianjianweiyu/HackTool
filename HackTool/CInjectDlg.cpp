// CInjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CInjectDlg.h"
#include "afxdialogex.h"
#include <atlconv.h>
#include <TlHelp32.h>




// CInjectDlg 对话框

IMPLEMENT_DYNAMIC(CInjectDlg, CDialogEx)

CInjectDlg::CInjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INJECTION, pParent)
	, m_btnRadio(0)
	, m_Edit_DllName(_T("请拖入MyHookDll.dll"))
	, m_Edit_ProcName(_T(""))
{

}

CInjectDlg::~CInjectDlg()
{

}

void CInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_btnRadio);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_DllName);
	DDX_Text(pDX, IDC_EDIT2, m_Edit_ProcName);
}


BEGIN_MESSAGE_MAP(CInjectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CInjectDlg::OnBnClickedButton1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_RADIO1, &CInjectDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CInjectDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CInjectDlg::OnBnClickedRadio1)
	ON_EN_SETFOCUS(IDC_EDIT2, &CInjectDlg::OnEnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT1, &CInjectDlg::OnEnSetfocusEdit1)
END_MESSAGE_MAP()


//单击开始注入按钮-注入dll
void CInjectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取注入的方式
	UpdateData(TRUE);

	//检测是否为dll
	LPTSTR pszExtension = PathFindExtension(m_Edit_DllName);
	if (lstrcmp(pszExtension, L".dll") != 0)
	{
		MessageBox(_T("请先拖拽有效的dll文件"));
		return;
	}

	//检测文件是否存在
	if (GetFileAttributes(m_Edit_DllName) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(_T("文件不存在 请重试！"));
		return;
	}

	//根据注入的方式不同选择不同的方法进行注入
	switch (m_btnRadio)
	{
		//全局钩子注入
	case 0:
	{
		//定义函数指针和函数指针变量
		typedef BOOL(*ty_SetGlobalHook)();
		ty_SetGlobalHook fpSetGlobalHook = NULL;

		//获取要注入的DLL的加载基址
		m_hDll= LoadLibrary(m_Edit_DllName);
		if (!m_hDll)
		{
			MessageBox(L"Dll加载失败");
		}

		//获取设置全局钩子函数的函数地址。赋值给函数指针变量
		fpSetGlobalHook = (ty_SetGlobalHook)GetProcAddress(m_hDll, "SetGlobalHook");
		if (!fpSetGlobalHook)
		{
			MessageBox(L"加载设置全局钩子函数地址失败");
		}

		//设置全局钩子
		BOOL bRet = fpSetGlobalHook();
		if (bRet)
		{
			MessageBox(L"全局钩子设置成功");
		}
		else
		{
			MessageBox(L"全局钩子设置失败");
		}
		break;
	}
		//远程线程注入
	case 1:
	{
		//判断进程ID是否为空
		if (m_Edit_ProcName == "" || m_Edit_ProcName == L"请输入需要注入的进程ID")
		{
			MessageBox(L"请输入需要注入的进程ID");
			break;
		}                                                  
		DWORD nPid = _ttoi(m_Edit_ProcName);
		USES_CONVERSION;
		char* pszDllFileName = T2A(m_Edit_DllName);
		
		//提升Debug权限
		EnableDebugPrivilege(GetCurrentProcess(), (char*)SE_DEBUG_NAME);
		//远程线程注入DLL
		CreateRemoteThreadInjectDll(nPid, pszDllFileName);
		//远程线程注入DLL(突破SESSION 0)
		//ZwCreateThreadExInjectDll(nPid, pszDllFileName);
		break;
	}
		//APC注入
	case 2:
	{
		//判断进程名是否为空
		if (m_Edit_ProcName == "" || m_Edit_ProcName == L"请输入需要注入的进程名")
		{
			MessageBox(L"请输入需要注入的进程名");
			break;
		}
		USES_CONVERSION;
		char* pszDllFileName = T2A(m_Edit_DllName);
		char* pszProcName = T2A(m_Edit_ProcName);
		//提升权限
		EnableDebugPrivilege(GetCurrentProcess(), (char*)SE_DEBUG_NAME);

		//APC注入
		ApcInjectDll(pszProcName, pszDllFileName);
		break;
	}
	}
}

//响应拖拽文件
void CInjectDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//过滤后缀名是否为DLL
	LPTSTR pszExtension = PathFindExtension(szPath);
	if (lstrcmp(pszExtension, L".dll") == 0)
	{
		//显示到控件上
		m_Edit_DllName = szPath;
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(L"请拖入有效的DLL文件");
		OnBnClickedRadio1();
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//响应单选框按钮点击事件-3个按钮共用一个处理函数
void CInjectDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	switch (m_btnRadio)
	{
	case 0:
	{
		m_Edit_DllName = _T("请拖入MyHookDll.dll");
		//隐藏进程名/ID静态文本控件与编辑框控件
		GetDlgItem(IDC_EDIT2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_ProcId)->ShowWindow(FALSE);
		break;
	}
	case 1:
	{
		m_Edit_DllName = _T("请拖入RemoteThreadDll.dll");
		m_Edit_ProcName = _T("请输入需要注入的进程ID");
		//显示进程名/ID静态文本控件与编辑框控件
		GetDlgItem(IDC_EDIT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_ProcId)->ShowWindow(TRUE);
		break;
	}
	case 2:
	{
		m_Edit_DllName = _T("请拖入RemoteThreadDll.dll");
		m_Edit_ProcName = _T("请输入需要注入的进程名");
		//显示进程名/ID静态文本控件与编辑框控件
		GetDlgItem(IDC_EDIT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_ProcId)->ShowWindow(TRUE);
		break;
	}
	}
	UpdateData(FALSE);
}

BOOL CInjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//隐藏进程名/ID静态文本控件与编辑框控件
	GetDlgItem(IDC_EDIT2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_ProcId)->ShowWindow(FALSE);

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);       // 0x0049 == WM_COPYGLOBALDATA

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//响应进程名/ID编辑框获取焦点
void CInjectDlg::OnEnSetfocusEdit2()
{
	// TODO: 在此添加控件通知处理程序代码
	//清空编辑快内容
	m_Edit_ProcName = "";
	UpdateData(FALSE);
}

//响应DLL路径编辑框获取焦点
void CInjectDlg::OnEnSetfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Edit_DllName = "";
	UpdateData(FALSE);
}

//普通远程线程注入Dll
BOOL CInjectDlg::CreateRemoteThreadInjectDll(DWORD dwProcessId, char* pszDllFileName)
{
	// 1.打开目标进程
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,		// 打开权限
		FALSE,					// 是否继承
		dwProcessId);			// 进程PID
	if (NULL == hProcess)
	{
		MessageBox(L"打开目标进程失败！");
		return FALSE;
	}

	// 2.在目标进程中申请空间
	LPVOID lpPathAddr = VirtualAllocEx(
		hProcess,					// 目标进程句柄
		0,							// 指定申请地址
		strlen(pszDllFileName)+1,	// 申请空间大小
		MEM_RESERVE | MEM_COMMIT,	// 内存的状态
		PAGE_READWRITE);			// 内存属性
	if (NULL == lpPathAddr)
	{
		MessageBox(L"在目标进程中申请空间失败！");
		CloseHandle(hProcess);
		return FALSE;
	}

	// 3.在目标进程中写入Dll路径
	SIZE_T dwWriteSize = 0;
	if (FALSE == WriteProcessMemory(
		hProcess,					// 目标进程句柄
		lpPathAddr,					// 目标进程地址
		pszDllFileName,					// 写入的缓冲区
		strlen(pszDllFileName) + 1,	// 缓冲区大小
		&dwWriteSize))				// 实际写入大小
	{
		MessageBox(L"目标进程中写入Dll路径失败！");
		CloseHandle(hProcess);
		return FALSE;
	}

	//获取LoadLibraryA的函数地址
	//FARPROC可以自适应32位与64位
	FARPROC pFuncProcAddr = GetProcAddress(GetModuleHandle((LPCWSTR)L"kernel32.dll"),"LoadLibraryA");
	if (NULL == pFuncProcAddr)
	{
		MessageBox(L"获取LoadLibrary函数地址失败！");
		CloseHandle(hProcess);
		return FALSE;
	}

	// 4.在目标进程中创建线程
	HANDLE hThread = CreateRemoteThread(
		hProcess,					// 目标进程句柄
		NULL,						// 安全属性
		NULL,						// 栈大小
		(PTHREAD_START_ROUTINE)pFuncProcAddr,	// 回调函数
		lpPathAddr,					// 回调函数参数
		NULL,						// 标志
		NULL						// 线程ID
	);
	if (NULL == hThread)
	{
		MessageBox(L"目标进程中创建线程失败！");
		CloseHandle(hProcess);
		return FALSE;
	}

	// 5.等待线程结束
	WaitForSingleObject(hThread, -1);

	DWORD code;
	GetExitCodeThread(hThread, &code);
	code = GetLastError();

	// 6.清理环境
	VirtualFreeEx(hProcess, lpPathAddr, 0, MEM_RELEASE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return TRUE;
}

//提权
BOOL CInjectDlg::EnableDebugPrivilege(HANDLE hProcess,char* pszPrivilegesName)
{
	HANDLE hToken = NULL;
	//令牌权限结构体
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;

	//打开进程令牌并获取进程令牌句柄
	BOOL bRet = OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
	if (FALSE == bRet)
	{
		MessageBox(L"打开进程令牌失败！");
		return FALSE;
	}
	//获取本地系统的 pszPrivilegesName 特权的LUID值
	bRet = LookupPrivilegeValue(NULL, (LPCWSTR)pszPrivilegesName, &tp.Privileges[0].Luid);
	if (FALSE == bRet)
	{
		MessageBox(L"获取LUID值失败！");
		CloseHandle(hToken);
		hToken = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	//设置提升权限信息
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//提升进程令牌访问权限
	bRet = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	if (FALSE == bRet)
	{
		MessageBox(L"提升进程令牌访问权限失败！");
		CloseHandle(hToken);
		hToken = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	else
	{
		//根据错误码判断是否是特权都设置成功
		DWORD dwRet = GetLastError();
		if (ERROR_SUCCESS == dwRet)
		{
			CloseHandle(hToken);
			hToken = INVALID_HANDLE_VALUE;
			return TRUE;
		}
		else if(ERROR_NOT_ALL_ASSIGNED == dwRet)
		{
			MessageBox(L"提升权限失败！，请以管理员身份运行");
			CloseHandle(hToken);
			hToken = INVALID_HANDLE_VALUE;
			return FALSE;
		}
	}
	CloseHandle(hToken);
	hToken = INVALID_HANDLE_VALUE;
	return FALSE;
}

//突破SESSION 0隔离的远程线程注入Dll
BOOL CInjectDlg::ZwCreateThreadExInjectDll(DWORD dwProcessId, char* pszDllFileName)
{
	// 1.打开目标进程
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,		// 打开权限
		FALSE,					// 是否继承
		dwProcessId);			// 进程PID
	if (NULL == hProcess)
	{
		MessageBox(L"打开目标进程失败！");
		return FALSE;
	}

	// 2.在目标进程中申请空间
	LPVOID lpPathAddr = VirtualAllocEx(
		hProcess,					// 目标进程句柄
		0,							// 指定申请地址
		strlen(pszDllFileName) + 1,	// 申请空间大小
		MEM_RESERVE | MEM_COMMIT,	// 内存的状态
		PAGE_READWRITE);			// 内存属性
	if (NULL == lpPathAddr)
	{
		MessageBox(L"在目标进程中申请空间失败！");
		CloseHandle(hProcess);
		return FALSE;
	}

	// 3.在目标进程中写入Dll路径
	if (FALSE == WriteProcessMemory(
		hProcess,					// 目标进程句柄
		lpPathAddr,					// 目标进程地址
		pszDllFileName,					// 写入的缓冲区
		strlen(pszDllFileName) + 1,	// 缓冲区大小
		NULL))				// 实际写入大小
	{
		MessageBox(L"目标进程中写入Dll路径失败！");
		CloseHandle(hProcess);
		return FALSE;
	}

	//4.加载ntdll.dll
	HMODULE hNtdll = LoadLibrary(L"ntdll.dll");
	if (NULL == hNtdll)
	{
		MessageBox(L"加载ntdll.dll失败！");
		CloseHandle(hProcess);
		return FALSE;
	}

	//5.获取LoadLibraryA的函数地址
	//FARPROC可以自适应32位与64位
	FARPROC pFuncProcAddr = GetProcAddress(GetModuleHandle((LPCWSTR)L"kernel32.dll"), "LoadLibraryA");
	if (NULL == pFuncProcAddr)
	{
		MessageBox(L"获取LoadLibrary函数地址失败！");
		CloseHandle(hProcess);
		return FALSE;
	}

	//6.获取ZwCreateThreadEx函数地址,该函数在32位与64位下原型不同
	//_WIN64用来判断编译环境 ，_WIN32用来判断是否是Windows系统
#ifdef _WIN64
	typedef DWORD(WINAPI* typedef_ZwCreateThreadEx)(
		PHANDLE ThreadHandle,
		ACCESS_MASK DesiredAccess,
		LPVOID ObjectAttributes,
		HANDLE ProcessHandle,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		ULONG CreateThreadFlags,
		SIZE_T ZeroBits,
		SIZE_T StackSize,
		SIZE_T MaximumStackSize,
		LPVOID pUnkown
		);
#else
	typedef DWORD(WINAPI* typedef_ZwCreateThreadEx)(
		PHANDLE ThreadHandle,
		ACCESS_MASK DesiredAccess,
		LPVOID ObjectAttributes,
		HANDLE ProcessHandle,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		BOOL CreateSuspended,
		DWORD dwStackSize,
		DWORD dw1,
		DWORD dw2,
		LPVOID pUnkown
		);
#endif 
	typedef_ZwCreateThreadEx ZwCreateThreadEx =
		(typedef_ZwCreateThreadEx)GetProcAddress(hNtdll, "ZwCreateThreadEx");
	if (NULL == ZwCreateThreadEx)
	{
		MessageBox(L"获取ZwCreateThreadEx函数地址失败！");
		CloseHandle(hProcess);
		return FALSE;
	}
	//7.在目标进程中创建线程
	HANDLE hRemoteThread = NULL;
	DWORD dwStatus = ZwCreateThreadEx(&hRemoteThread, PROCESS_ALL_ACCESS, NULL, hProcess,
		(LPTHREAD_START_ROUTINE)pFuncProcAddr, lpPathAddr, 0, 0, 0, 0, NULL);
	if (NULL == hRemoteThread)
	{
		MessageBox(L"目标进程中创建线程失败！");
		CloseHandle(hProcess);
		return FALSE;
	}

	// 8.等待线程结束
	WaitForSingleObject(hRemoteThread, -1);

	// 9.清理环境
	VirtualFreeEx(hProcess, lpPathAddr, 0, MEM_RELEASE);
	CloseHandle(hRemoteThread);
	CloseHandle(hProcess);
	FreeLibrary(hNtdll);
	return TRUE;
}

//APC注入
BOOL CInjectDlg::ApcInjectDll(char* pszProcessName, char* pszDllFileName)
{
	DWORD* pThreadId = NULL;
	DWORD dwThreadIdLength = 0;
	//根据进程名获取PID
	DWORD dwProcessId = GetProcessIdByProcessName(pszProcessName);
	if (dwProcessId <= 0)
	{	
		return FALSE;
	}
	//根据PID获取所有相应的线程ID
	DWORD bRet = GetAllThreadIdByProcessId(dwProcessId, &pThreadId, &dwThreadIdLength);
	if (FALSE == bRet)
	{
		return FALSE;
	}
	//打开目标进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		MessageBox(L"打开目标进程失败！");
		return FALSE;
	}
	// 在目标进程中申请空间
	LPVOID lpPathAddr = VirtualAllocEx(
		hProcess,					// 目标进程句柄
		0,							// 指定申请地址
		strlen(pszDllFileName) + 1,	// 申请空间大小
		MEM_RESERVE | MEM_COMMIT,	// 内存的状态
		PAGE_READWRITE);			// 内存属性
	if (NULL == lpPathAddr)
	{
		MessageBox(L"在目标进程中申请空间失败！");
		CloseHandle(hProcess);
		return FALSE;
	}
	// 3.在目标进程中写入Dll路径
	if (FALSE == WriteProcessMemory(
		hProcess,					// 目标进程句柄
		lpPathAddr,					// 目标进程地址
		pszDllFileName,					// 写入的缓冲区
		strlen(pszDllFileName) + 1,	// 缓冲区大小
		NULL))				// 实际写入大小
	{
		MessageBox(L"目标进程中写入Dll路径失败！");
		CloseHandle(hProcess);
		return FALSE;
	}
	//5.获取LoadLibraryA的函数地址
	//FARPROC可以自适应32位与64位
	FARPROC pFuncProcAddr = GetProcAddress(GetModuleHandle((LPCWSTR)L"kernel32.dll"), "LoadLibraryA");
	if (NULL == pFuncProcAddr)
	{
		MessageBox(L"获取LoadLibrary函数地址失败！");
		CloseHandle(hProcess);
		return FALSE;
	}
	// 遍历线程, 插入APC
	for (int i = 0; i < dwThreadIdLength; i++)
	{
		// 打开线程
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pThreadId[i]);
		if (hThread)
		{
			// 插入APC
			QueueUserAPC((PAPCFUNC)pFuncProcAddr, hThread, (ULONG_PTR)lpPathAddr);
			// 关闭线程句柄
			CloseHandle(hThread);
			hThread = NULL;
		}
	}

	return TRUE;
}

//根据进程名获取PID
DWORD CInjectDlg::GetProcessIdByProcessName(char* pszProcessName)
{
	//1.创建进程快照
	HANDLE hSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS,			//遍历进程快照1
		0);							//进程PID
	if (NULL == hSnap)
	{
		MessageBox(L"创建进程快照失败！");
		return 0;
	}

	//2.获取第一条进程快照信息
	PROCESSENTRY32  stcPe = { sizeof(stcPe) };
	if (Process32First(hSnap, &stcPe))
	{

		//3.循环遍历进程Next
		do {

			//获取快照信息
			USES_CONVERSION;
			CString ProcessName = A2T(pszProcessName);
			if (!lstrcmp(stcPe.szExeFile, ProcessName))
			{
				//4.关闭句柄
				CloseHandle(hSnap);
				return stcPe.th32ProcessID;
			}

		} while (Process32Next(hSnap, &stcPe));

	}
	
	//4.关闭句柄
	CloseHandle(hSnap);
	return 0;
}

//根据PID获取所有相应的线程ID
BOOL CInjectDlg::GetAllThreadIdByProcessId(DWORD dwProcessId, DWORD** ppThreadId, DWORD* pdwThreadIdLength)
{
	DWORD* pThreadId = NULL;
	//统计线程个数
	DWORD dwThreadIdLength = 0;
	//默认情况下，一个线程的栈要预留1M的内存空间, 
	//而一个进程中可用的内存空间只有2G，所以理论
	//上一个进程中最多可以开2048个线程
	DWORD dwBuffLength = 2048;

	//申请内存
	pThreadId = new DWORD[dwBuffLength];
	if (pThreadId == NULL)
	{
		MessageBox(L"申请内存失败！");
		return FALSE;
	}
	//将申请的控件初始化为0
	RtlZeroMemory(pThreadId, (dwBuffLength * sizeof(DWORD)));
	//1.创建线程快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (NULL == hSnap)
	{
		MessageBox(L"创建线程快照失败！");
		delete[] pThreadId;
		pThreadId = NULL;
		return FALSE;
	}

	//2.第一次遍历线程
	THREADENTRY32 th32 = { sizeof(th32) };
	if (Thread32First(hSnap, &th32))
	{
	//3.循环遍历线程
		do
		{
			//判断该线程是否属于这个进程
			if (th32.th32OwnerProcessID == dwProcessId)
			{
				pThreadId[dwThreadIdLength] = th32.th32ThreadID;
				dwThreadIdLength++;
			}

		} while (Thread32Next(hSnap, &th32));

		CloseHandle(hSnap);
		*ppThreadId = pThreadId;
		*pdwThreadIdLength = dwThreadIdLength;
		return TRUE;
	}
	else
	{
		MessageBox(L"创建线程快照失败！");
		delete[] pThreadId;
		pThreadId = NULL;
		CloseHandle(hSnap);
		return FALSE;
	}

}