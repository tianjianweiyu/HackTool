// CStartSelfDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CStartSelfDlg.h"
#include "afxdialogex.h"
#include "CMyTaskSchedule.h"
#include <winsvc.h>


// CStartSelfDlg 对话框

IMPLEMENT_DYNAMIC(CStartSelfDlg, CDialogEx)

CStartSelfDlg::CStartSelfDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_STARTSELF, pParent)
	, m_Edit_FilePath(_T("请将文件拖拽到此处"))
	, m_Edit_TipMsg(_T("计划任务与系统服务需以管理员权限启动本程序!"))
{

}

CStartSelfDlg::~CStartSelfDlg()
{
}

void CStartSelfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_FilePath);
	DDX_Text(pDX, IDC_EDIT2, m_Edit_TipMsg);
}


BEGIN_MESSAGE_MAP(CStartSelfDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CStartSelfDlg::OnBnClickedButton1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CStartSelfDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CStartSelfDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CStartSelfDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CStartSelfDlg 消息处理程序

//单击注册表按钮-通过注册表实现开机自启动
void CStartSelfDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//检测文件是否存在
	if (GetFileAttributes(m_Edit_FilePath) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}
	
	//默认权限
	HKEY hKey;
	//打开注册表键
	if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey))
	{
		m_Edit_TipMsg = "打开注册表键失败!";
		UpdateData(FALSE);
		return;
	}
	USES_CONVERSION;
	char* pszFilePath = T2A(m_Edit_FilePath);
	DWORD size0fFilePath = strlen(pszFilePath);

	//修改注册表值，实现开机自启动
	if (ERROR_SUCCESS != RegSetValueExA(hKey, "hl15pb", 0, REG_SZ, (BYTE*)pszFilePath,1+size0fFilePath))
	{
		RegCloseKey(hKey);
		m_Edit_TipMsg = "修改注册表值失败!";
		UpdateData(FALSE);
		return;
	}
	//关闭注册表键
	RegCloseKey(hKey);
	m_Edit_TipMsg = "已将目标程序设置到以下注册表项!\r\n";
	m_Edit_TipMsg += "\\HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows-\r\n";
	m_Edit_TipMsg += "\\CurrentVersion\\Run\\hl15pb\r\n";
	m_Edit_TipMsg += "可自行查看或删除\r\n";
	UpdateData(FALSE);
	return;
}


BOOL CStartSelfDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);       // 0x0049 == WM_COPYGLOBALDATA

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//响应拖拽文件
void CStartSelfDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//过滤后缀名是否为exe
	LPTSTR pszExtension = PathFindExtension(szPath);
	if (lstrcmp(pszExtension, L".exe") == 0)
	{
		//显示到控件上
		m_Edit_FilePath = szPath;
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(L"请拖入有效的exe文件");
		//显示到控件上
		m_Edit_FilePath = _T("请将文件拖拽到此处");
		UpdateData(FALSE);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//单击快速启动目录按钮-通过快速启动目录实现开机自启动
void CStartSelfDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//检测文件是否存在
	if (GetFileAttributes(m_Edit_FilePath) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}

	//清空文本框的内容
	m_Edit_TipMsg = "";

	BOOL bRet = FALSE;
	char szStartupPath[MAX_PATH] = { 0 };
	char szDestFilePath[MAX_PATH] = { 0 };
	USES_CONVERSION;
	char* pszFilePath = T2A(m_Edit_FilePath);

	// 获取快速启动目录的路径
	bRet = SHGetSpecialFolderPathA(NULL, szStartupPath, CSIDL_STARTUP, TRUE);
	printf("szStartupPath=%s\n", szStartupPath);
	if (FALSE == bRet)
	{
		m_Edit_TipMsg = "获取快速启动目录的路径失败!";
		UpdateData(FALSE);
		return;
	}

	//创建快捷方式
	//初始化供调用线程使用的COM库，设置线程的并发模型，并在需要时为该线程创建一个新的单元
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
	{
		IShellLink *pisl;
		hr = CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
		if (SUCCEEDED(hr))
		{
			IPersistFile* pIPF;

			//这里是我们要创建快捷方式的原始文件地址
			pisl->SetPath(m_Edit_FilePath);
			hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
			if (SUCCEEDED(hr))
			{
				//获取文件名
				PathStripPathA(pszFilePath);
				//拼接路径
				wsprintfA(szDestFilePath, "%s\\%s", szStartupPath, pszFilePath);
				//注意路径后缀名要从exe改为lnk
				int nLen = strlen(szDestFilePath);
				szDestFilePath[nLen - 3] = 'l';
				szDestFilePath[nLen - 2] = 'n';
				szDestFilePath[nLen - 1] = 'k';
				WCHAR* szwDestFilePath = A2W(szDestFilePath);
				//这里是我们要创建快捷方式的目标地址
				pIPF->Save(szwDestFilePath, FALSE);
				//减少COM对象上接口的引用计数
				pIPF->Release();
			}
			//减少COM对象上接口的引用计数
			pisl->Release();
		}
		//关闭当前线程上的COM库，卸载该线程加载的所有DLL
		CoUninitialize();
		m_Edit_TipMsg = "目标程序已添加到以下快速启动目录\r\n";
		m_Edit_TipMsg += szStartupPath;
		m_Edit_TipMsg += "\r\n";
		m_Edit_TipMsg += "可自行查看或删除\r\n";
		UpdateData(FALSE);
	}
}

//单击计划任务按钮-通过计划任务实现开机自启动
void CStartSelfDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	//检测文件是否存在
	if (GetFileAttributes(m_Edit_FilePath) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}

	//清空文本框的内容
	m_Edit_TipMsg = "";

	//创建计划任务
	CMyTaskSchedule myTask;
	USES_CONVERSION;
	BOOL bRet = myTask.NewTask("hl15pb", W2A(m_Edit_FilePath), "", "hl15pb");
	if (FALSE == bRet)
	{
		m_Edit_TipMsg = "创建计划任务失败!\r\n";
	}
	else
	{
		m_Edit_TipMsg = "任务计划设置成功\r\n";
		m_Edit_TipMsg += "任务名：hl15pb\r\n";
	}
	UpdateData(FALSE);
}

//单击系统服务按钮-通过系统服务实现开机自启动
void CStartSelfDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	//检测文件是否存在
	if (GetFileAttributes(m_Edit_FilePath) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在 请重试");
		return;
	}

	//清空文本框的内容
	m_Edit_TipMsg = "";

	USES_CONVERSION;
	//创建服务
	BOOL bRet = SystemServiceOperate(W2A(m_Edit_FilePath), 0);
	if (FALSE == bRet)
	{
		m_Edit_TipMsg += "创建服务失败!";
	}
	else
	{
		m_Edit_TipMsg += "创建服务成功!";
	}

	//启动服务
	bRet = SystemServiceOperate(W2A(m_Edit_FilePath), 1);
	if (FALSE == bRet)
	{
		m_Edit_TipMsg += "启动服务失败!";
		UpdateData(FALSE);
		return;
	}
	else
	{
		m_Edit_TipMsg += "启动服务成功!";
	}
	UpdateData(FALSE);
}

// 0 创建服务    1 启动服务    2 停止服务    3 删除服务
BOOL CStartSelfDlg::SystemServiceOperate(char *lpszDriverPath, int iOperateType)
{
	BOOL bRet = TRUE;
	char szName[MAX_PATH] = { 0 };
	strcpy_s(szName, lpszDriverPath);
	// 过滤掉文件目录，获取文件名
	PathStripPathA(szName);
	SC_HANDLE shOSCM = NULL, shCS = NULL;
	SERVICE_STATUS ss;
	DWORD dwErrorCode = 0;
	BOOL bSuccess = FALSE;
	// 打开服务控制管理器数据库
	shOSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!shOSCM)
	{
		m_Edit_TipMsg = "打开服务控制管理器数据库失败!\r\n";
		return FALSE;
	}
	if (0 != iOperateType)
	{
		// 打开一个已经存在的服务
		shCS = OpenServiceA(shOSCM, szName, SERVICE_ALL_ACCESS);
		if (!shCS)
		{
			m_Edit_TipMsg = "打开服务失败!\r\n";
			CloseServiceHandle(shOSCM);
			shOSCM = NULL;
			return FALSE;
		}
	}
	switch (iOperateType)
	{
	case 0:
	{
		// 创建服务
		// SERVICE_AUTO_START   随系统自动启动
		// SERVICE_DEMAND_START 手动启动
		shCS = CreateServiceA(shOSCM, szName, szName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			lpszDriverPath, NULL, NULL, NULL, NULL, NULL);
		if (!shCS)
		{
			m_Edit_TipMsg = "创建服务失败!\r\n";
			bRet = FALSE;
		}
		break;
	}
	case 1:
	{
		// 启动服务
		if (!StartService(shCS, 0, NULL))
		{
			m_Edit_TipMsg = "启动服务失败!\r\n";
			bRet = FALSE;
		}
		break;
	}
	case 2:
	{
		// 停止服务
		if (!ControlService(shCS, SERVICE_CONTROL_STOP, &ss))
		{
			m_Edit_TipMsg = "停止服务失败!\r\n";
			bRet = FALSE;
		}
		break;
	}
	case 3:
	{
		// 删除服务
		if (!DeleteService(shCS))
		{
			m_Edit_TipMsg = "删除服务失败!\r\n";
			bRet = FALSE;
		}
		break;
	}
	default:
		break;
	}
	// 关闭句柄
	if (shCS)
	{
		CloseServiceHandle(shCS);
		shCS = NULL;
	}
	if (shOSCM)
	{
		CloseServiceHandle(shOSCM);
		shOSCM = NULL;
	}
	return bRet;
}