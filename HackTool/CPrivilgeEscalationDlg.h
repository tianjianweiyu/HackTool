#pragma once


// CPrivilgeEscalationDlg 对话框

class CPrivilgeEscalationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrivilgeEscalationDlg)

public:
	CPrivilgeEscalationDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPrivilgeEscalationDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRIVILGEESCALATION_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	//************************************
	// 函数名:  CPrivilgeEscalationDlg::EnableDebugPrivilege
	// 返回类型:   BOOL
	// 功能: 提升进程访问令牌权限
	// 参数1: 需要提升权限的进程句柄
	// 参数2: 特权名称
	//************************************
	BOOL EnableDebugPrivilege(HANDLE hProcess, char* pszPrivilegesName);
	afx_msg void OnBnClickedButton2();
};
