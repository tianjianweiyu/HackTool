// CEnumProcessDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CEnumProcessDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CEnumProcessDlg 对话框

IMPLEMENT_DYNAMIC(CEnumProcessDlg, CDialogEx)

CEnumProcessDlg::CEnumProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENUMPROCESS_DIALOG, pParent)
{

}

CEnumProcessDlg::~CEnumProcessDlg()
{
}

void CEnumProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_List);
}


BEGIN_MESSAGE_MAP(CEnumProcessDlg, CDialogEx)
END_MESSAGE_MAP()


// CEnumProcessDlg 消息处理程序


BOOL CEnumProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	BOOL bRet = EnumProcess();
	if (bRet == FALSE)
	{
		MessageBox(_T("进程遍历失败！"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//遍历进程
BOOL CEnumProcessDlg::EnumProcess()
{
	//包含进程信息的结构体 这个结构体的长度在使用之前必须设置
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };

	//创建一个快照 参数一 要获取的快照类型 参数2 进程ID,此时置0
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("CreateToolhelp32Snapshot Error"));
		return FALSE;
	}

	// 设置列表的扩展风格
	m_List.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_CHECKBOXES
		| LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_List.InsertColumn(0, _T("进程名："),0,200);
	m_List.InsertColumn(1, _T("ID"), 0, 100);
	m_List.InsertColumn(2, _T("优先级"), 0, 100);
	m_List.InsertColumn(3, _T("父进程ID"), 0, 100);
	m_List.InsertColumn(4, _T("持有线程数"), 0, 100);

	//找到第一个进程的信息 参数1 快照句柄 参数2 进程信息
	if (Process32First(hSnap, &pe) == TRUE)
	{
		
		CString temp;
		int i = 0;
		//循环遍历其他进程
		do
		{
		
			//把进程名添加到行
			m_List.InsertItem(i, pe.szExeFile);
			//把ID添加到行
			temp.Format(_T("%d"), pe.th32ProcessID);
			m_List.SetItemText(i, 1, temp);
			//把优先级添加到行
			temp.Format(_T("%d"), pe.pcPriClassBase);
			m_List.SetItemText(i, 2, temp);
			//把父进程ID添加到行
			temp.Format(_T("%d"), pe.th32ParentProcessID);
			m_List.SetItemText(i, 3, temp);
			//把线程数添加到行
			temp.Format(_T("%d"), pe.cntThreads);
			m_List.SetItemText(i, 4, temp);
			i++;
		} while (Process32Next(hSnap, &pe));
	}

	return TRUE;
}