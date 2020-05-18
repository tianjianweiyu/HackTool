// CDiscDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CDiscDlg.h"
#include "afxdialogex.h"



char* TypeName[7] = {
	"未知设备",
	"无效根路径",
	"可移动设备",
	"不可移动设备",
	"网络驱动器",
	"CD-ROM 驱动器",
	"随机存储器",
};


// CDiscDlg 对话框

IMPLEMENT_DYNAMIC(CDiscDlg, CDialogEx)

CDiscDlg::CDiscDlg(TCHAR* name,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DISC_DIALOG, pParent)
	, m_Disc(_T(""))
	, m_DiscName(_T(""))
	, m_DiscSystem(_T(""))
	, m_DiscFreeSpace(_T(""))
	, m_DiscTotalSpace(_T(""))
	, m_DiscInfo(_T(""))
	, m_path(name)
{

}

CDiscDlg::~CDiscDlg()
{
}

void CDiscDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Disc);
	DDX_Text(pDX, IDC_EDIT4, m_DiscName);
	DDX_Text(pDX, IDC_EDIT5, m_DiscSystem);
	DDX_Text(pDX, IDC_EDIT6, m_DiscFreeSpace);
	DDX_Text(pDX, IDC_EDIT7, m_DiscTotalSpace);
	DDX_Text(pDX, IDC_EDIT8, m_DiscInfo);
}


BEGIN_MESSAGE_MAP(CDiscDlg, CDialogEx)
END_MESSAGE_MAP()


// CDiscDlg 消息处理程序


BOOL CDiscDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//显示磁盘
	m_Disc = m_path;

	//获取磁盘类型
	UINT type = GetDriveType(m_path);
	//显示磁盘信息
	m_DiscInfo = TypeName[type];

	//获取硬盘信息
	TCHAR Name[MAX_PATH] = { 0 };
	TCHAR System[MAX_PATH] = { 0 };
	GetVolumeInformation(m_path, Name, MAX_PATH, NULL, NULL, NULL, System, MAX_PATH);
	//显示磁盘名
	m_DiscName = Name;
	if (m_DiscName == _T(""));
	{
		m_DiscName = _T("本地磁盘");
	}
	//显示磁盘系统
	m_DiscSystem = System;

	//显示磁盘空间
	DWORD a = 0, b = 0, c = 0, d = 0;
	GetDiskFreeSpace(m_path, &a, &b, &c,&d);
	double FreeSize = (c / 1024.0)*a*b / 1024 / 1024;
	double TotalSize = (d / 1024.0)*a*b / 1024 / 1024;
	m_DiscFreeSpace.Format(_T("%.2lfGB"), FreeSize);
	m_DiscTotalSpace.Format(_T("%.2lfGB"), TotalSize);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
