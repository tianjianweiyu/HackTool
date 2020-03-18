// CBasicsDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CBasicsDlg.h"
#include "afxdialogex.h"

extern HANDLE g_hMutex;

// CBasicsDlg 对话框

IMPLEMENT_DYNAMIC(CBasicsDlg, CDialogEx)

CBasicsDlg::CBasicsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BASICS, pParent)
{

}

CBasicsDlg::~CBasicsDlg()
{
}

void CBasicsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBasicsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBasicsDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CBasicsDlg 消息处理程序

//单击防多开按钮-开启/关闭防多开(默认是开启的)
void CBasicsDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CloseHandle(g_hMutex);	//关闭互斥体对象
	

}
