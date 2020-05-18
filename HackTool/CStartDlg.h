#pragma once


// CStart 对话框

class CStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartDlg)

public:
	CStartDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStartDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_START };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_Edit_FilePath;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	INT m_Radio_Start;

	//************************************
	// 函数名:  CStartDlg::WinExec_Start
	// 返回类型:   BOOL
	// 功能: 以WinExec的方式创建进程
	// 参数1: char * pszExePath	exe文件路径
	// 参数2: UINT uiCmdShow	显示方式
	//************************************
	BOOL WinExec_Start(char *pszExePath, UINT uiCmdShow);

	//************************************
	// 函数名:  CStartDlg::ShellExecute_Test
	// 返回类型:   BOOL
	// 功能: 以ShellExecute的方式创建进程
	// 参数1: CString pszExePath exe文件路径
	// 参数2: UINT uiCmdShow	显示方式
	//************************************
	BOOL ShellExecute_Start(CString pszExePath, UINT uiCmdShow);

	//************************************
	// 函数名:  CStartDlg::CreateProcess_Start
	// 返回类型:   BOOL
	// 功能: 以CreateProcess方式创建进程
	// 参数1: char * pszExePath exe文件路径
	// 参数2: UINT uiCmdShow	显示方式
	//************************************
	BOOL CreateProcess_Start(char* pszExePath, UINT uiCmdShow);
	afx_msg void OnBnClickedButton2();

	//************************************
	// 函数名:  CStartDlg::MmLoadLibrary
	// 返回类型:   LPVOID
	// 功能: 模拟LoadLibrary加载内存文件到进程中
	// 参数1: LPVOID lpData	文件基址
	// 参数2: BOOL IsExe	文件属性标志，TRUE表示exe文件，FALSE表示dll文件
	//************************************
	LPVOID MmLoadLibrary(LPVOID lpData,BOOL IsExe);

	//************************************
	// 函数名:  CStartDlg::GetSizeOfImage
	// 返回类型:   DWORD
	// 功能: 获取文件映像大小
	// 参数1: LPVOID lpData	文件基址
	//************************************
	DWORD GetSizeOfImage(LPVOID lpData);

	//************************************
	// 函数名:  CStartDlg::MmMapFile
	// 返回类型:   BOOL
	// 功能: 将内存DLL数据按映像对齐大小（SectionAlignment）映射到刚刚申请的内存中
	// 参数1: LPVOID lpData	文件基址
	// 参数2: LPVOID lpBaseAddress	申请的内存的首地址
	//************************************
	BOOL MmMapFile(LPVOID lpData, LPVOID lpBaseAddress);

	//************************************
	// 函数名:  CStartDlg::DoRelocationTable
	// 返回类型:   BOOL
	// 功能: 修改PE文件的重定位表信息
	// 参数1: LPVOID lpBaseAddress	映像对齐后的文件基址
	//************************************
	BOOL DoRelocationTable(LPVOID lpBaseAddress);

	//************************************
	// 函数名:  CStartDlg::DoImportTable
	// 返回类型:   BOOL
	// 功能: 填写PE文件的导入表信息
	// 参数1: LPVOID lpBaseAddress	映像对齐后的文件基址
	//************************************
	BOOL DoImportTable(LPVOID lpBaseAddress);

	//************************************
	// 函数名:  CStartDlg::SetImageBase
	// 返回类型:   BOOL
	// 功能: 修改PE文件的加载基址IMAGE_NT_HEADERS.OptionalHeader.ImageBase
	// 参数1: LPVOID lpBaseAddress	映像对齐后的文件基址
	//************************************
	BOOL SetImageBase(LPVOID lpBaseAddress);

	//************************************
	// 函数名:  CStartDlg::CallDllMain
	// 返回类型:   BOOL
	// 功能: 调用PE文件的入口函数
	// 参数1: LPVOID lpBaseAddress	映像对齐后的文件基址
	// 参数2: BOOL IsExe	文件属性标志，TRUE表示exe文件，FALSE表示dll文件
	//************************************
	BOOL CallDllMain(LPVOID lpBaseAddress, BOOL IsExe);

	//************************************
	// 函数名:  CStartDlg::LoadMyResource
	// 返回类型:   LPVOID
	// 功能: 加载资源到内存
	// 参数1: UINT uiResourceName	资源名
	// 参数1: char * lpszResourceType	资源类型
	//************************************
	LPVOID LoadMyResource(UINT uiResourceName, char* lpszResourceType);
	CComboBox m_Combo_Start;
};
