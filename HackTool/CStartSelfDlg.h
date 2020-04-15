#pragma once


// CStartSelfDlg 对话框

class CStartSelfDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartSelfDlg)

public:
	CStartSelfDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStartSelfDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STARTSELF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_Edit_FilePath;
	CString m_Edit_TipMsg;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	//************************************
	// 函数名:  CStartSelfDlg::SystemServiceOperate
	// 返回类型:   BOOL
	// 功能: 创建/启动/停止/删除系统服务
	// 参数1: char * lpszDriverPath	服务名称
	// 参数1: int iOperateType	操作码 0表示创建服务，1启动，2停止，3删除	
	//************************************
	BOOL SystemServiceOperate(char *lpszDriverPath, int iOperateType);
};
