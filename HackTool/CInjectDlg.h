#pragma once


// CInjectDlg 对话框

class CInjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInjectDlg)

public:
	CInjectDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInjectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INJECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	INT m_btnRadio;
	afx_msg void OnBnClickedButton1();		//开始注入按钮
	CString m_Edit_DllName;
	afx_msg void OnDropFiles(HDROP hDropInfo);		//响应拖拽文件
	afx_msg void OnBnClickedRadio1();	//响应单选框按钮点击事件-3个按钮共用一个处理函数
	CString m_Edit_ProcName;
	virtual BOOL OnInitDialog();
	HMODULE m_hDll;		//要注入的Dll的加载基址
	afx_msg void OnEnSetfocusEdit2();	//响应进程名/ID编辑框获取焦点
	afx_msg void OnEnSetfocusEdit1();	//响应DLL路径编辑框获取焦点


	//************************************
	// 函数名:  CInjectDlg::CreateRemoteThreadInjectDll
	// 返回类型:   BOOL
	// 功能: 远程线程注入Dll到目标进程
	// 参数1: 目标进程的ID
	// 参数2: DLL名
	//************************************
	BOOL CreateRemoteThreadInjectDll(DWORD dwProcessId, char* pszDllFileName);

	//************************************
	// 函数名:  CInjectDlg::EnableDebugPrivilege
	// 返回类型:   BOOL
	// 功能: 提升进程访问令牌权限
	// 参数1: 需要提升权限的进程句柄
	// 参数2: 特权名称
	//************************************
	BOOL EnableDebugPrivilege(HANDLE hProcess, char* pszPrivilegesName);

	//************************************
	// 函数名:  CInjectDlg::ZwCreateThreadExInjectDll
	// 返回类型:   BOOL
	// 功能: 远程线程注入Dll到目标进程(可以突破SESSION 0)
	// 参数1: 目标进程的ID
	// 参数1: DLL名
	//************************************
	BOOL ZwCreateThreadExInjectDll(DWORD dwProcessId, char* pszDllFileName);

	//************************************
	// 函数名:  CInjectDlg::ApcInjectDll
	// 返回类型:   BOOL
	// 功能: APC注入
	// 参数1: char * pszProcessName  目标进程名
	// 参数1: char * pszDllFileName	 Dll名
	//************************************
	BOOL ApcInjectDll(char* pszProcessName, char* pszDllFileName);

	//************************************
	// 函数名:  CInjectDlg::GetProcessIdByProcessName
	// 返回类型:   DWORD
	// 功能: 根据进程名获取PID
	// 参数1: char * pszProcessName 进程名
	//************************************
	DWORD GetProcessIdByProcessName(char* pszProcessName);

	//************************************
	// 函数名:  CInjectDlg::GetAllThreadIdByProcessId
	// 返回类型:   BOOL
	// 功能: 根据PID获取所有相应的线程ID
	// 参数1: DWORD dwProcessId 进程ID
	// 参数1: DWORD * * ppThreadId	用于接收线程id数组
	// 参数1: DWORD * pdwThreadIdLength	用于接收线程个数
	//************************************
	BOOL GetAllThreadIdByProcessId(DWORD dwProcessId, DWORD** ppThreadId, DWORD* pdwThreadIdLength);
};
