// CFileAttrDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CFileAttrDlg.h"
#include "afxdialogex.h"


// CFileAttrDlg 对话框

IMPLEMENT_DYNAMIC(CFileAttrDlg, CDialogEx)

CFileAttrDlg::CFileAttrDlg(TCHAR* file, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEATTR_DIALOG, pParent)
	, m_Name(_T(""))
	, m_Type(_T(""))
	, m_Path(_T(""))
	, m_CreateTime(_T(""))
	, m_LVisitTime(_T(""))
	, m_LChangeTime(_T(""))
	, m_Read(FALSE)
	, m_Hide(FALSE)
	,m_CurrentFile(file)	//获取文件完整路径
{

}

CFileAttrDlg::~CFileAttrDlg()
{
}

void CFileAttrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT4, m_Type);
	DDX_Text(pDX, IDC_EDIT5, m_Path);
	DDX_Text(pDX, IDC_EDIT6, m_CreateTime);
	DDX_Text(pDX, IDC_EDIT7, m_LVisitTime);
	DDX_Text(pDX, IDC_EDIT8, m_LChangeTime);
	DDX_Check(pDX, IDC_CHECK1, m_Read);
	DDX_Check(pDX, IDC_CHECK2, m_Hide);
}


BEGIN_MESSAGE_MAP(CFileAttrDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileAttrDlg 消息处理程序


BOOL CFileAttrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//获取文件路径
	TCHAR FullPath[MAX_PATH] = { 0 };
	TCHAR* Name;
	GetFullPathName(m_CurrentFile, MAX_PATH, FullPath, &Name);
	//显示文件名
	m_Name = Name;
	//显示文件路径
	m_Path = FullPath;

	//获取文件类型
	SHFILEINFO ShInfo = { 0 };
	SHGetFileInfo(FullPath, 0, &ShInfo, sizeof(ShInfo), SHGFI_TYPENAME);
	m_Type = ShInfo.szTypeName;

	//获取文件信息
	WIN32_FIND_DATA FileInfo = { 0 };
	FindFirstFile(FullPath, &FileInfo);
	//将文件时间转为系统时间
	SYSTEMTIME SystemTime = { 0 };
	FileTimeToSystemTime(&FileInfo.ftCreationTime, &SystemTime);
	//显示创建时间
	m_CreateTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
		SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

	//将文件时间转为系统时间
	FileTimeToSystemTime(&FileInfo.ftLastAccessTime, &SystemTime);
	//显示最后访问时间
	m_LVisitTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
		SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

	//将文件时间转为系统时间
	FileTimeToSystemTime(&FileInfo.ftLastWriteTime, &SystemTime);
	//显示最后修改时间
	m_LChangeTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
		SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

	//设置只读复选框
	if (FileInfo.dwFileAttributes&FILE_ATTRIBUTE_READONLY)
	{
		m_Read = TRUE;
	}
	if (FileInfo.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)
	{
		m_Hide = TRUE;
	}

	UpdateData(FALSE);
		

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
