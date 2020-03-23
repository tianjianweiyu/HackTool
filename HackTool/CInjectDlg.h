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
};
