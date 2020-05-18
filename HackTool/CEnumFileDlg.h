#pragma once


// CEnumFileDlg 对话框

class CEnumFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnumFileDlg)

public:
	CEnumFileDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEnumFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENUMFILE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//遍历文件
	BOOL EnumFiles(TCHAR *pszDirectory);
public:
	virtual BOOL OnInitDialog();
	CComboBox m_Combox;
	CListCtrl m_ListCtrl;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_FilePath;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPopFileAttr();
};
