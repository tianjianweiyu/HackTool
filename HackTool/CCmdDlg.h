#pragma once


// CCmdDlg 对话框

class CCmdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCmdDlg)

public:
	CCmdDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCmdDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//************************************
	// 函数名：CCmdDlg::PipeCmd
	// 返回类型：BOOL
	// 功能:	执行CMD命令, 并获取执行结果数据
	// 参数1：char *pszCmd	cmd命令
	// 参数2：char *pszResultBuffer	接收CMD结果的缓冲区
	// 参数3：DWORD dwResultBufferSize	缓冲区大小(缓冲区要大点，不然接收到的结果不全)
	//************************************
	BOOL PipeCmd(char *pszCmd, char *pszResultBuffer, DWORD dwResultBufferSize);
public:
	CString m_Cmd;
	CString m_Result;
	afx_msg void OnBnClickedButton1();
};
