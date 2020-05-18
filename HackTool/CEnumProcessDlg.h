#pragma once


// CEnumProcessDlg 对话框

class CEnumProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnumProcessDlg)

public:
	CEnumProcessDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEnumProcessDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENUMPROCESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//遍历进程
	BOOL EnumProcess();

public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
};
