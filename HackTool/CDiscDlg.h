#pragma once


// CDiscDlg 对话框

class CDiscDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiscDlg)

public:
	CDiscDlg(TCHAR* name,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDiscDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Disc;
	CString m_DiscName;
	CString m_DiscSystem;
	CString m_DiscFreeSpace;
	CString m_DiscTotalSpace;
	CString m_DiscInfo;

	CString m_path;
	virtual BOOL OnInitDialog();
};
