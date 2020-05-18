#pragma once


// CFileAttrDlg 对话框

class CFileAttrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileAttrDlg)

public:
	CFileAttrDlg(TCHAR* file, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFileAttrDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEATTR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	CString m_Type;
	CString m_Path;
	CString m_CreateTime;
	CString m_LVisitTime;
	CString m_LChangeTime;
	BOOL m_Read;
	BOOL m_Hide;
	CString m_CurrentFile;
	virtual BOOL OnInitDialog();
};
