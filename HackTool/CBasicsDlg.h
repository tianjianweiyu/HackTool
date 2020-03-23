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
	afx_msg void OnBnClickedButton1();		//防多开按钮
	afx_msg void OnBnClickedButton2();		//释放资源按钮
	CString m_Edit_Basic;				//提示
	CString m_FreeResTipMsg;			//释放资源操作过程中的提示
	CString m_PreMulOpenTipMsg;			//防多开是否开启的提示

	//释放资源

	//************************************
	// 函数名:  CBasicsDlg::FreeMyResource
	// 返回类型:   BOOL
	// 功能:释放资源将资源保存为文件
	// 参数1: 资源名
	// 参数2: 资源类型
	// 参数3: 文件名
	//************************************
	BOOL FreeMyResource(UINT uiResourceName, char* lpszResourceType, char* lpszSaveFileName);
};
