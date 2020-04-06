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
	// 参数1: UINT uiCmdShow	显示方式
	//************************************
	BOOL WinExec_Start(char *pszExePath, UINT uiCmdShow);

	//************************************
	// 函数名:  CStartDlg::ShellExecute_Test
	// 返回类型:   BOOL
	// 功能: 以ShellExecute的方式创建进程
	// 参数1: CString pszExePath exe文件路径
	// 参数1: UINT uiCmdShow	显示方式
	//************************************
	BOOL ShellExecute_Start(CString pszExePath, UINT uiCmdShow);

	//************************************
	// 函数名:  CStartDlg::CreateProcess_Start
	// 返回类型:   BOOL
	// 功能: 以CreateProcess方式创建进程
	// 参数1: char * pszExePath exe文件路径
	// 参数1: UINT uiCmdShow	显示方式
	//************************************
	BOOL CreateProcess_Start(char* pszExePath, UINT uiCmdShow);
	afx_msg void OnBnClickedButton2();

	LPVOID MmLoadLibrary(LPVOID lpData,BOOL IsExe);

	DWORD GetSizeOfImage(LPVOID lpData);

	BOOL MmMapFile(LPVOID lpData, LPVOID lpBaseAddress);

	BOOL DoRelocationTable(LPVOID lpBaseAddress);

	BOOL DoImportTable(LPVOID lpBaseAddress);

	BOOL SetImageBase(LPVOID lpBaseAddress);

	BOOL CallDllMain(LPVOID lpBaseAddress, BOOL IsExe);

	HANDLE LoadMyResource(UINT uiResourceName, char* lpszResourceType);
	CComboBox m_Combo_Start;
};
