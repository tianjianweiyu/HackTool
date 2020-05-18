// CFileMonitDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CFileMonitDlg.h"
#include "afxdialogex.h"
#include <minwinbase.h>


// CFileMonitDlg 对话框

IMPLEMENT_DYNAMIC(CFileMonitDlg, CDialogEx)

CFileMonitDlg::CFileMonitDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEMONIT_DIALOG, pParent)
	, m_Edit_Path(_T("请拖入要监控的文件"))
{

}

CFileMonitDlg::~CFileMonitDlg()
{
}

void CFileMonitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_Path);
	DDX_Control(pDX, IDC_EDIT2, m_Monit_Tip);
}


BEGIN_MESSAGE_MAP(CFileMonitDlg, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CFileMonitDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CFileMonitDlg 消息处理程序


BOOL CFileMonitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);       // 0x0049 == WM_COPYGLOBALDATA

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CFileMonitDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//显示到控件上
	m_Edit_Path = szPath;
	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}

//开启监控按钮
void CFileMonitDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	//检测文件是否存在
	if (GetFileAttributes(m_Edit_Path) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(_T("文件不存在 请重试！"));
		return;
	}

	USES_CONVERSION;
	//创建线程
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorFileThreadProc, this, 0, NULL);

	return;
}


// 目录监控多线程
UINT MonitorFileThreadProc(LPVOID lpVoid)
{
	CFileMonitDlg* ptrData = (CFileMonitDlg*)lpVoid;
	CString temp;
	temp = ptrData->m_Edit_Path + L"\\";

	// 打开目录, 获取文件句柄
	HANDLE hDirectory = ::CreateFile(temp.GetBuffer(), FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (INVALID_HANDLE_VALUE == hDirectory)
	{

		MessageBoxA(0, "CreateFileA Error", "Error", MB_OK);
		return 1;
	}

	DWORD dwBufferSize = 4096;
	// 申请一个足够大的缓冲区 
	BYTE* pBuf = new BYTE[dwBufferSize];
	if (NULL == pBuf)
	{
		MessageBoxA(0, "new Error", "Error", MB_OK);
		return 2;
	}

	FILE_NOTIFY_INFORMATION* pFileNotifyInfo = (FILE_NOTIFY_INFORMATION *)pBuf;
	
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	CString res = L"";
	// 开始循环设置监控
	do
	{
		::RtlZeroMemory(pFileNotifyInfo, dwBufferSize);
		// 设置监控目录
		bRet = ::ReadDirectoryChangesW(hDirectory,
			pFileNotifyInfo,
			dwBufferSize,
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_ATTRIBUTES |
			FILE_NOTIFY_CHANGE_LAST_WRITE,
			&dwRet,
			NULL, NULL);
		if (FALSE == bRet)
		{
			MessageBoxA(0, "ReadDirectoryChangesW Error", "Error", MB_OK);
			break;
		}

		// 判断操作类型并显示
		switch (pFileNotifyInfo->Action)
		{
			//新增文件
		case FILE_ACTION_ADDED:
		{
			temp.Format(_T("%s:%s\r\n"), _T("新增文件"), pFileNotifyInfo->FileName);
			res += temp + L"\r\n";
			break;
		}
			//删除文件
		case FILE_ACTION_REMOVED:
		{
			temp.Format(_T("%s:%s\r\n"), _T("删除文件"), pFileNotifyInfo->FileName);
			res += temp + L"\r\n";
			break;
		}
			//修改文件
		case FILE_ACTION_MODIFIED:
		{
			temp.Format(_T("%s:%s\r\n"), _T("修改文件"), pFileNotifyInfo->FileName);
			res += temp + L"\r\n";
			break;
		}
			//重命名文件
		case FILE_ACTION_RENAMED_OLD_NAME:
		{
			temp.Format(_T("%s:%s\r\n"), _T("重命名前文件"), pFileNotifyInfo->FileName);
			res += temp + L"\r\n";
			break;
		}
		case FILE_ACTION_RENAMED_NEW_NAME:
		{
			temp.Format(_T("%s:%s\r\n"), _T("重命名后文件"), pFileNotifyInfo->FileName);
			res += temp + L"\r\n";
			break;
		}
		default:
			break;
		}

		SetDlgItemText(ptrData->m_hWnd,IDC_EDIT2, res);

	} while (bRet);



	// 关闭句柄, 释放内存
	CloseHandle(hDirectory);
	delete[] pBuf;

	return 0;
}