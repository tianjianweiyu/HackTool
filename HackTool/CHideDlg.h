#pragma once


// CHideDlg 对话框

class CHideDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHideDlg)

public:
	CHideDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHideDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HIDE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	//************************************
	// 函数名：CHideDlg::DisguiseProcess
	// 返回类型：BOOL
	// 功能:	修改指定PEB中的路径和命令行信息, 实现进程伪装
	// 参数1：DWORD dwProcessId 需要伪装的程序的PID
	// 参数2：wchar_t *lpwszPath 伪装之后程序的路径
	//************************************
	BOOL DisguiseProcess(DWORD dwProcessId, wchar_t *lpwszPath);

	//判断当前系统是否是32位

	//************************************
	// 函数名：CHideDlg::IsSystem32
	// 返回类型：BOOL
	// 功能:	判断当前系统是否是32位 是就返回TRUE ,不是就返回FALSE
	//************************************
	BOOL IsSystem32();

	//************************************
	// 函数名：CHideDlg::ReplaceProcess
	// 返回类型：BOOL
	// 功能: 使用ShellCode替换目标进程
	// 参数1：char *pszFilePath		目标进程路径
	// 参数2：PVOID pReplaceData	Shellcode首地址
	// 参数3：DWORD dwReplaceDataSize	Shellcode大小(字节)
	// 参数4：DWORD dwRunOffset		Shellcode中开始代码相对首地址的偏移
	//************************************
	BOOL ReplaceProcess(char *pszFilePath, PVOID pReplaceData, DWORD dwReplaceDataSize, DWORD dwRunOffset);

	afx_msg void OnBnClickedButton2();
	CString m_Path;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();

	//************************************
	// 函数名：CHideDlg::GetProcessIdByProcessName
	// 返回类型：DWORD	进程ID
	// 功能:	根据进程名获取PID
	// 参数1：char* pszProcessName 进程名
	//************************************
	DWORD GetProcessIdByProcessName(char* pszProcessName);

	afx_msg void OnBnClickedButton4();
};
