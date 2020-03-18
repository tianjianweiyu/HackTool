#pragma once


// CBasicsDlg 对话框

class CBasicsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBasicsDlg)

public:
	CBasicsDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBasicsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BASICS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
