// CCmdDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CCmdDlg.h"
#include "afxdialogex.h"


// CCmdDlg 对话框

IMPLEMENT_DYNAMIC(CCmdDlg, CDialogEx)

CCmdDlg::CCmdDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMD_DIALOG, pParent)
	, m_Cmd(_T(""))
	, m_Result(_T(""))
{

}

CCmdDlg::~CCmdDlg()
{
}

void CCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Cmd);
	DDX_Text(pDX, IDC_EDIT2, m_Result);
}


BEGIN_MESSAGE_MAP(CCmdDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCmdDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCmdDlg 消息处理程序

//执行按钮
void CCmdDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	//检测命令是否为空
	if (m_Cmd == "")
	{
		MessageBox(_T("CMD命令不能为空"));
		return;
	}

	//清空之前的显示结果
	m_Result = "";

	//获取需要执行的CMD命令
	char szResultBuffer[4096] = { 0 };
	DWORD dwResultBufferSize = 4096;

	//执行CMD命令 并获取执行结果数据
	USES_CONVERSION;
	if (FALSE == PipeCmd(W2A(m_Cmd), szResultBuffer, 4096))
	{
		MessageBox(_T("pipe cmd error"));
		return;
	}
	//显示到界面
	m_Result += A2W(szResultBuffer);

	UpdateData(FALSE);
}


// 执行CMD命令, 并获取执行结果数据
BOOL CCmdDlg::PipeCmd(char *pszCmd, char *pszResultBuffer, DWORD dwResultBufferSize)
{
	SECURITY_ATTRIBUTES securityAttributes = { 0 };
	// 设定管道的安全属性
	securityAttributes.bInheritHandle = TRUE;
	securityAttributes.nLength = sizeof(securityAttributes);
	securityAttributes.lpSecurityDescriptor = NULL;

	// 创建匿名管道
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;
	BOOL bRet = ::CreatePipe(&hReadPipe, &hWritePipe, &securityAttributes, 0);
	if (FALSE == bRet)
	{
		MessageBox(_T("CreatePipe Error"));
		return FALSE;
	}

	STARTUPINFO si = { 0 };
	// 设置新进程参数
	si.cb = sizeof(si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	// 创建新进程执行命令, 将执行结果写入匿名管道中
	PROCESS_INFORMATION pi = { 0 };
	USES_CONVERSION;
	bRet = ::CreateProcess(NULL, A2W(pszCmd), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (FALSE == bRet)
	{
		MessageBox(_T("CreateProcess Error"));
	}

	// 等待命令执行结束
	::WaitForSingleObject(pi.hThread, INFINITE);
	::WaitForSingleObject(pi.hProcess, INFINITE);

	// 从匿名管道中读取结果到输出缓冲区
	::RtlZeroMemory(pszResultBuffer, dwResultBufferSize);
	::ReadFile(hReadPipe, pszResultBuffer, dwResultBufferSize, NULL, NULL);

	// 关闭句柄, 释放内存
	::CloseHandle(pi.hThread);
	::CloseHandle(pi.hProcess);
	::CloseHandle(hWritePipe);
	::CloseHandle(hReadPipe);

	return TRUE;
}