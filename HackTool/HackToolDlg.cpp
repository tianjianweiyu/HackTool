
// HackToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "HackTool.h"
#include "HackToolDlg.h"
#include "afxdialogex.h"
#include "CBasicsDlg.h"
#include "CInjectDlg.h"
#include "CStartDlg.h"
#include "CStartSelfDlg.h"
#include "CPrivilgeEscalationDlg.h"
#include "CHideDlg.h"
#include "CCondenseDlg.h"
#include "CEncryptionDlg.h"
#include "CFunction.h"
#include "CHelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE g_hMutex = NULL;			//保存防多开的互斥体对象的句柄


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHackToolDlg 对话框



CHackToolDlg::CHackToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HACKTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHackToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHackToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CHackToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CHackToolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CHackToolDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CHackToolDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CHackToolDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CHackToolDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CHackToolDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CHackToolDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CHackToolDlg::OnBnClickedButton10)
	ON_COMMAND(ID_32773, &CHackToolDlg::On32773)
	ON_COMMAND(ID_32774, &CHackToolDlg::On32774)
END_MESSAGE_MAP()


// CHackToolDlg 消息处理程序

BOOL CHackToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//默认使用互斥体防多开
	g_hMutex = CreateMutex(NULL, FALSE, _T("HackTool"));

	//如果创建互斥体失败或互斥体已经存在
	if (g_hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(_T("请勿双开本程序，即将退出"));
		exit(0);//正常退出
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHackToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHackToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHackToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//单击基础技术按钮-弹出基础技术对话框
void CHackToolDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CBasicsDlg MyBasicsDlg;		//创建窗口框架
	MyBasicsDlg.DoModal();		//创建模态对话框窗口

}

//单击注入技术按钮-弹出注入技术对话框
void CHackToolDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	CInjectDlg MyInjectDlg;		//创建窗口框架
	MyInjectDlg.DoModal();		//创建模态对话框窗口
}


//单击启动技术按钮-弹出启动技术对话框
void CHackToolDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CStartDlg MyStartDlg;		//创建窗口框架
	MyStartDlg.DoModal();		//创建模态对话框窗口
}

//单击自启动技术按钮-弹出自启动技术对话框
void CHackToolDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CStartSelfDlg MyStartSelfDlg;		//创建窗口框架
	MyStartSelfDlg.DoModal();		//创建模态对话框窗口
}

//单击提权技术按钮-弹出提权技术对话框
void CHackToolDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CPrivilgeEscalationDlg MyPrivilgeEscalationDlg;		//创建窗口框架
	MyPrivilgeEscalationDlg.DoModal();		//创建模态对话框窗口
}

//单击隐藏技术按钮-弹出隐藏技术对话框
void CHackToolDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CHideDlg MyHideDlg;	//创建窗口框架
	MyHideDlg.DoModal();	//创建模态对话框窗口
}

//单击压缩技术按钮-弹出压缩技术对话框
void CHackToolDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CCondenseDlg MyCondenseDlg;	//创建窗口框架
	MyCondenseDlg.DoModal();	//创建模态对话框窗口
}

//单击加密技术按钮-弹出加密技术对话框
void CHackToolDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	CEncryptionDlg MyEncryptionDlg;	//创建窗口框架
	MyEncryptionDlg.DoModal();	//创建模态对话框窗口
}

//单击功能技术按钮-弹出功能技术对话框
void CHackToolDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	CFunction MyFunctionDlg;		//创建窗口框架
	MyFunctionDlg.DoModal();	//创建模态对话框窗口
}


//菜单帮助按钮
void CHackToolDlg::On32773()
{
	// TODO: 在此添加命令处理程序代码
	CHelpDlg MyHelpDlh;		//创建窗口框架
	MyHelpDlh.DoModal();	//创建模态对话框窗口
}

//菜单说明按钮
void CHackToolDlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox(_T("Create By XiaoBai"));
}
