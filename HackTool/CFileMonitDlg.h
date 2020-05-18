#pragma once


// CFileMonitDlg 对话框

class CFileMonitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileMonitDlg)

public:
	CFileMonitDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFileMonitDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEMONIT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()



public:
	CString m_Edit_Path;
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButton1();
	CEdit m_Monit_Tip;
};


// 目录监控多线程
UINT MonitorFileThreadProc(LPVOID lpVoid);