// CInjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CInjectDlg.h"
#include "afxdialogex.h"


// CInjectDlg 对话框

IMPLEMENT_DYNAMIC(CInjectDlg, CDialogEx)

CInjectDlg::CInjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INJECTION, pParent)
	, m_btnRadio(0)
	, m_Edit_DllName(_T("请拖入MyHookDll.dll"))
	, m_Edit_ProcName(_T(""))
{

}

CInjectDlg::~CInjectDlg()
{

}

void CInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_btnRadio);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_DllName);
	DDX_Text(pDX, IDC_EDIT2, m_Edit_ProcName);
}


BEGIN_MESSAGE_MAP(CInjectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CInjectDlg::OnBnClickedButton1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_RADIO1, &CInjectDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CInjectDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CInjectDlg::OnBnClickedRadio1)
END_MESSAGE_MAP()


//单击开始注入按钮-注入dll
void CInjectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//检测文件是否存在
	if (GetFileAttributes(m_Edit_DllName) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(L"文件不存在");
		return;
	}

	//获取注入的方式
	UpdateData(TRUE);
	//根据注入的方式不同选择不同的方法进行注入
	switch (m_btnRadio)
	{
		//全局钩子注入
	case 0:
	{
		//定义函数指针和函数指针变量
		typedef BOOL(*ty_SetGlobalHook)();
		ty_SetGlobalHook fpSetGlobalHook = NULL;

		//获取要注入的DLL的加载基址
		m_hDll= LoadLibrary(m_Edit_DllName);
		if (!m_hDll)
		{
			MessageBox(L"Dll加载失败");
		}

		//获取设置全局钩子函数的函数地址。赋值给函数指针变量
		fpSetGlobalHook = (ty_SetGlobalHook)GetProcAddress(m_hDll, "SetGlobalHook");
		if (!fpSetGlobalHook)
		{
			MessageBox(L"加载设置全局钩子函数地址失败");
		}

		//设置全局钩子
		BOOL bRet = fpSetGlobalHook();
		if (bRet)
		{
			MessageBox(L"全局钩子设置成功");
		}
		else
		{
			MessageBox(L"全局钩子设置失败");
		}
		break;
	}
		//远程线程注入
	case 1:
	{
		break;
	}
		//APC注入
	case 2:
	{
		break;
	}
	}
}

//响应拖拽文件
void CInjectDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnDropFiles(hDropInfo);

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//过滤后缀名是否为DLL
	LPTSTR pszExtension = PathFindExtension(szPath);
	if (lstrcmp(pszExtension, L".dll") == 0)
	{
		//显示到控件上
		m_Edit_DllName = szPath;
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(L"请拖入有效的DLL文件");
		//显示到控件上
		m_Edit_DllName = _T("请拖入MyHookDll.dll");
		UpdateData(FALSE);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//响应单选框按钮点击事件-3个按钮共用一个处理函数
void CInjectDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	switch (m_btnRadio)
	{
	case 0:
	{
		m_Edit_DllName = _T("请拖入MyHookDll.dll");
		//隐藏进程名/ID静态文本控件与编辑框控件
		GetDlgItem(IDC_EDIT2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_ProcId)->ShowWindow(FALSE);
		break;
	}
	case 1:
	{
		m_Edit_DllName = _T("请拖入RemThrDll.dll");
		m_Edit_ProcName = _T("请输入需要注入的进程ID");
		//显示进程名/ID静态文本控件与编辑框控件
		GetDlgItem(IDC_EDIT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_ProcId)->ShowWindow(TRUE);
		break;
	}
	case 2:
	{
		m_Edit_DllName = _T("请拖入APCDll.dll");
		m_Edit_ProcName = _T("请输入需要注入的进程名");
		//显示进程名/ID静态文本控件与编辑框控件
		GetDlgItem(IDC_EDIT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_ProcId)->ShowWindow(TRUE);
		break;
	}
	}
	UpdateData(FALSE);
}

BOOL CInjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//隐藏进程名/ID静态文本控件与编辑框控件
	GetDlgItem(IDC_EDIT2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_ProcId)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


