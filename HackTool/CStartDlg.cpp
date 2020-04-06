// CStart.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CStartDlg.h"
#include "afxdialogex.h"
#include <winnt.h>

//定义DllMain函数指针
typedef BOOL(__stdcall *typedef_DllMain)(HINSTANCE hInstance, DWORD dwReson, LPVOID lpReserced);

// CStart 对话框

IMPLEMENT_DYNAMIC(CStartDlg, CDialogEx)

CStartDlg::CStartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_START, pParent)
	, m_Edit_FilePath(_T("请将文件拖拽到此处"))
	, m_Radio_Start(0)
{

}

CStartDlg::~CStartDlg()
{
}

void CStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_FilePath);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio_Start);
	DDX_Control(pDX, IDC_COMBO1, m_Combo_Start);
}


BEGIN_MESSAGE_MAP(CStartDlg, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CStartDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CStartDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CStart 消息处理程序

//响应拖拽文件
void CStartDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//过滤后缀名是否为exe
	LPTSTR pszExtension = PathFindExtension(szPath);
	if (lstrcmp(pszExtension, L".exe") == 0)
	{
		//显示到控件上
		m_Edit_FilePath = szPath;
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(L"请拖入有效的exe文件");
		//显示到控件上
		m_Edit_FilePath = _T("请将文件拖拽到此处");
		UpdateData(FALSE);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}


BOOL CStartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);       // 0x0049 == WM_COPYGLOBALDATA

	//给下拉框添加初始项
	m_Combo_Start.AddString(L"dll 文件");
	m_Combo_Start.AddString(L"exe 文件");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//单击创建进程按钮 - 根据选择的创建方式创建进程
void CStartDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//检测文件路径是否为空
	if (m_Edit_FilePath == L"" || m_Edit_FilePath == L"请将文件拖拽到此处")
	{
		MessageBox(L"请先拖拽文件到窗口");
		return;
	}
	//检测文件是否存
	if (GetFileAttributes(m_Edit_FilePath) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试！");
		return;
	}

	BOOL bRet = FALSE;
	//根据选择的方式创建进程
	switch (m_Radio_Start)
	{
		//使用WinExec创建进程
	case 0:
	{
		USES_CONVERSION;
		char* pszExePath = T2A(m_Edit_FilePath);
		bRet = WinExec_Start(pszExePath, SW_SHOWNORMAL);
		if (bRet)
		{
			MessageBox(L"WinExec方式创建进程成功！");
		}
		else
		{
			MessageBox(L"WinExec方式创建进程失败！");
		}
		break;
	}
		//使用ShellExecute创建进程
	case 1:
	{
		bRet = ShellExecute_Start(m_Edit_FilePath, SW_SHOWNORMAL);
		if (bRet)
		{
			MessageBox(L"ShellExecute方式创建进程成功！");
		}
		else
		{
			MessageBox(L"ShellExecute方式创建进程失败！");
		}
		break;
	}
		//使用CreateProcess创建进程
	case 2:
	{
		USES_CONVERSION;
		LPWSTR pszExePath = T2W(m_Edit_FilePath.GetBuffer(NULL));
		bRet = CreateProcess_Start((char*)pszExePath, SW_SHOWNORMAL);
		if (bRet)
		{
			MessageBox(L"CreateProcess方式创建进程成功！");
		}
		else
		{
			MessageBox(L"CreateProcess方式创建进程失败！");
		}
		break;
	}
	}
}

//以WinExec的方式创建进程
BOOL CStartDlg::WinExec_Start(char *pszExePath, UINT uiCmdShow)
{
	UINT uiRet = 0;
	//函数成功，返回值大于31
	uiRet = WinExec(pszExePath, uiCmdShow);
	if (31 < uiRet)
	{
		return TRUE;
	}
	return FALSE;
}

//以ShellExecute的方式创建进程
BOOL CStartDlg::ShellExecute_Start(CString pszExePath, UINT uiCmdShow)
{
	HINSTANCE hInstance = 0;
	//ShellExecute函数不仅可以运行exe文件，也可以运行已经关联的文件。
	//例如，可以打开网页、发送邮件、以默认程序打开文件、打开目录、打
	//印文件等。若返回值大于32，则表示执行成功，否则执行失败
	hInstance = ShellExecute(NULL, NULL, pszExePath, NULL, NULL, uiCmdShow);
	if (32 < (DWORD)hInstance)
	{
		return TRUE;
	}
	return FALSE;
}

//以CreateProcess方式创建进程
BOOL CStartDlg::CreateProcess_Start(char* pszExePath, UINT uiCmdShow)
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	BOOL bRet = FALSE;
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;  //指定wShowWindow成员有效
	si.wShowWindow = uiCmdShow;
	bRet = CreateProcess(NULL, (LPWSTR)pszExePath, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (bRet)
	{
		//不使用的句柄最好关掉
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return TRUE;
	}
	return FALSE;
}


//单击内存直接加载运行按钮-直接加载文件到内存并运行
void CStartDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	HANDLE hMmRes = NULL;
	BOOL IsExe = TRUE;
	//获取下拉框选择的索引
	int index = m_Combo_Start.GetCurSel();
	switch (index)
	{
	case 0:
	{
		//加载资源到内存
		hMmRes = LoadMyResource(IDR_MYRES3, (char*)L"MYRES");
		IsExe = FALSE;
		break;
	}
	case 1:
	{
		//加载资源到内存
		hMmRes = LoadMyResource(IDR_MYRES4, (char*)L"MYRES");
		break;
	}
	}

	MmLoadLibrary(hMmRes,IsExe);
}


// 模拟LoadLibrary加载内存DLL文件到进程中
LPVOID CStartDlg::MmLoadLibrary(LPVOID lpData,BOOL IsExe)
{
	LPVOID lpBaseAddress = NULL;
	// 获取映像大小
	DWORD dwSizeOfImage = GetSizeOfImage(lpData);
	// 在进程中申请一个可读、可写、可执行的内存块
	lpBaseAddress = VirtualAlloc(NULL, dwSizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (NULL == lpBaseAddress)
	{
		MessageBox(L"申请空间失败！");
		return NULL;
	}
	// 将申请的空间的数据全部置0
	RtlZeroMemory(lpBaseAddress, dwSizeOfImage);
	//修改页属性, 统一设置成属性PAGE_EXECUTE_READWRITE
	DWORD dwOldProtect = 0;
	if (FALSE == VirtualProtect(lpBaseAddress,
		dwSizeOfImage, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		MessageBox(L"修改页属性失败!");
		return NULL;
	}
	// 将内存DLL数据按映像对齐大小（SectionAlignment）映射到刚刚申请的内存中
	if (FALSE == MmMapFile(lpData, lpBaseAddress))
	{
		MessageBox(L"区段映射到内存失败!");
		return NULL;
	}
	//获取Dos头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	//获取NT头
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	DWORD Main = (DWORD)lpBaseAddress + pNtHeader->OptionalHeader.AddressOfEntryPoint;
	// 修改PE文件的重定位表信息
	if (FALSE == DoRelocationTable(lpBaseAddress))
	{
		MessageBox(L"修复重定位失败!");
		return NULL;
	}
	// 填写PE文件的导入表信息
	if (FALSE == DoImportTable(lpBaseAddress))
	{
		MessageBox(L"导入表填写失败!");
		return NULL;
	}
	// 修改PE文件的加载基址IMAGE_NT_HEADERS.OptionalHeader.ImageBase
	if (FALSE == SetImageBase(lpBaseAddress))
	{
		MessageBox(L"修改加载机制失败!");
		return NULL;
	}
	// 调用DLL的入口函数DllMain,函数地址即为PE文件的入口点AddressOfEntryPoint
	if (FALSE == CallDllMain(lpBaseAddress,IsExe))
	{
		MessageBox(L"调用入口函数失败!");
		return NULL;
	}
	return lpBaseAddress;
}

//获取映像大小
DWORD CStartDlg::GetSizeOfImage(LPVOID lpData)
{
	//获取Dos头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpData;
	//判断是否是有效的PE文件		0x5A4D
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(L"这不是一个PE文件!");
		return 0;
	}

	//获取NT头
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader+pDosHeader->e_lfanew);
	//判断是否是有效的PE文件		0x00004550
	if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(L"这不是一个PE文件!");
		return 0;
	}

	//获取文件映像大小
	return pNtHeader->OptionalHeader.SizeOfImage;
}

//将内存DLL数据按映像对齐大小（SectionAlignment）映射到刚刚申请的内存中
BOOL CStartDlg::MmMapFile(LPVOID lpData, LPVOID lpBaseAddress)
{
	//获取Dos头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpData;
	//获取NT头
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	//获取所有头部+区段表的大小
	DWORD dwSizeOfHeaders = pNtHeader->OptionalHeader.SizeOfHeaders;
	//获取区段数量
	WORD wNumberOfSections = pNtHeader->FileHeader.NumberOfSections;
	//获取区段表数组的首元素
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
	//将头部（包括区段表）拷贝到内存
	RtlCopyMemory(lpBaseAddress, lpData, dwSizeOfHeaders);
	LPVOID lpSrcMem = NULL;
	LPVOID lpDestMem = NULL;
	DWORD dwSizeOfRawData = 0;
	//循环加载所有区段
	for (WORD i = 0; i < wNumberOfSections; i++)
	{
		//过滤掉无效区段
		if (0 == pSectionHeader->VirtualAddress || 0 == pSectionHeader->SizeOfRawData)
		{
			pSectionHeader++;
			continue;
		}
		//获取区段在文件中的位置
		lpSrcMem = (LPVOID)((DWORD)lpData + pSectionHeader->PointerToRawData);
		//获取区段映射到内存中的位置
		lpDestMem = (LPVOID)((DWORD)lpBaseAddress + pSectionHeader->VirtualAddress);
		//获取区段在文件中的大小
		dwSizeOfRawData = pSectionHeader->SizeOfRawData;
		//将区段数据拷贝到内存中
		RtlCopyMemory(lpDestMem, lpSrcMem, dwSizeOfRawData);
		//获取下一个区段头（属性）
		pSectionHeader++;
	}
	return TRUE;
}

//修改PE文件的重定位表信息
BOOL CStartDlg::DoRelocationTable(LPVOID lpBaseAddress)
{
	//获取Dos头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	//获取NT头
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	//获取重定位表的地址
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pDosHeader + 
		pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

	//注意重定位表的位置可能和硬盘文件中的偏移地址不同，应该使用加载后的地址

	//判断是否有重定位表
	if ((PVOID)pReloc == (PVOID)pDosHeader)
	{
		//没有重定位表
		return TRUE;
	}

	int nNumberOfReloc = 0;
	WORD* pRelocData = NULL;
	DWORD* pAddress = NULL;
	//开始修复重定位
	while (pReloc->VirtualAddress != 0 && pReloc->SizeOfBlock != 0)
	{
		//计算本区域（每一个描述了4KB大小的区域的重定位信息）需要修正的重定位项的数量
		nNumberOfReloc = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);

		for (int i = 0; i < nNumberOfReloc; i++)
		{
			//获取IMAGE_BASE_RELOCATION结构后面的数据的地址
			pRelocData = (WORD*)((DWORD)pReloc + sizeof(IMAGE_BASE_RELOCATION));

			//每个WORD由两部分组成，高4位指出了重定位的类型，WINNT.H中的一系列IMAGE_REL_BASED_xxx定义了重定位类型的取值。
			//大部分重定位属性值都是0x3
			//低12位是相对于IMAGE_BASE_RELOCATION中第一个元素VirtualAddress描述位置的偏移
			//找出需要修正的地址
			if ((WORD)(pRelocData[i] & 0xF000) == 0x3000)
			{
				//获取需要修正数据的地址,	按位与计算优先级比加减乘除低
				pAddress = (DWORD*)((DWORD)pDosHeader + pReloc->VirtualAddress + (pRelocData[i] & 0x0FFF));
				//进行修改
				*pAddress += (DWORD)pDosHeader - pNtHeader->OptionalHeader.ImageBase;
			}
		}

		//下一个重定位块
		pReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pReloc + pReloc->SizeOfBlock);
	}
	return TRUE;
}

//填写PE文件的导入表信息
BOOL CStartDlg::DoImportTable(LPVOID lpBaseAddress)
{
	//获取Dos头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	//获取NT头
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	//获取导入表地址
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pDosHeader + 
		pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	char* pDllName = nullptr;
	HMODULE hDll = NULL;
	PIMAGE_THUNK_DATA pIat = NULL;
	FARPROC pFuncAddress = NULL;
	PIMAGE_IMPORT_BY_NAME pImportByName = NULL;

	//循环遍历导入表
	while (pImport->Name)
	{
		//获取导入表中的Dll名称
		pDllName = (char*)((DWORD)pDosHeader + pImport->Name);
		//检索Dll模块获取模块句柄
		hDll = GetModuleHandleA(pDllName);
		//获取失败
		if (NULL == hDll)
		{
			//加载Dll模块获取模块句柄
			hDll = LoadLibraryA(pDllName);
			//加载失败
			if (NULL == hDll)
			{
				pImport++;
				continue;
			}
		}
		
		//获取IAT
		pIat = (PIMAGE_THUNK_DATA)((DWORD)pDosHeader + pImport->FirstThunk);

		//遍历IAT中函数
		while (pIat->u1.Ordinal)
		{
			//判断导入的函数是名称导入还是序号导入
			//判断最高位是否为1，如果是1那么是序号导入
			if (pIat->u1.Ordinal & 0x80000000)
			{
				//获取函数地址
				pFuncAddress = GetProcAddress(hDll, (LPCSTR)(pIat->u1.Ordinal & 0x80000000));

			}
				//名称导入
			else
			{
				//获取IMAGE_IMPORT_BY_NAME结构
				pImportByName = (PIMAGE_IMPORT_BY_NAME)((DWORD)pDosHeader + pIat->u1.AddressOfData);
				//获取函数地址
				pFuncAddress = GetProcAddress(hDll, pImportByName->Name);
			}
			//将函数地址填入到IAT中
			pIat->u1.Function = (DWORD)pFuncAddress;
			pIat++;
		}
		pImport++;
	}

	return TRUE;
}

// 修改PE文件的加载基址IMAGE_NT_HEADERS.OptionalHeader.ImageBase
BOOL CStartDlg::SetImageBase(LPVOID lpBaseAddress)
{
	//获取Dos头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	//获取NT头
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	//修改默认加载基址
	pNtHeader->OptionalHeader.ImageBase = (DWORD)lpBaseAddress;
	return TRUE;
}

//调用DLL的入口函数DllMain
BOOL CStartDlg::CallDllMain(LPVOID lpBaseAddress,BOOL IsExe)
{
	//定义函数指针变量
	typedef_DllMain DllMain = NULL;

	//获取Dos头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	//获取NT头
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);

	BOOL bRet = TRUE;
	//如果是exe文件
	if (IsExe)
	{
		LPVOID  Main = (LPVOID)((DWORD)pDosHeader + pNtHeader->OptionalHeader.AddressOfEntryPoint);
		__asm {
			mov eax, Main;
			call eax;
		}
	}
	//dll 文件
	else {
		DllMain = (typedef_DllMain)((DWORD)pDosHeader + pNtHeader->OptionalHeader.AddressOfEntryPoint);
		//调用入口函数，附加进程DLL_PROCESS_ATTACH
		bRet = DllMain((HINSTANCE)lpBaseAddress, DLL_PROCESS_ATTACH, NULL);
	}

	return bRet;
}

//加载资源到内存
HANDLE CStartDlg::LoadMyResource(UINT uiResourceName, char* lpszResourceType)
{
	//获取指定模块里的资源
	HRSRC hRsrc = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(uiResourceName), (LPCWSTR)lpszResourceType);
	if (NULL == hRsrc)
	{
		MessageBox(L"获取资源失败!");
		return FALSE;
	}
	//获取资源大小
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (dwSize <= 0)
	{
		MessageBox(L"获取资源大小失败!");
		return FALSE;
	}
	//将资源加载到内存里
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		MessageBox(L"资源加载到内存失败!");
		return FALSE;
	}
	//锁定资源
	LPVOID lpVoid = LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		MessageBox(L"锁定资源失败!");
		return FALSE;
	}
	return hGlobal;
}
