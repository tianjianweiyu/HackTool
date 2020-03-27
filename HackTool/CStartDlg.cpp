// CStart.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CStartDlg.h"
#include "afxdialogex.h"


// CStart 对话框

IMPLEMENT_DYNAMIC(CStartDlg, CDialogEx)

CStartDlg::CStartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_START, pParent)
{

}

CStartDlg::~CStartDlg()
{
}

void CStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartDlg, CDialogEx)
END_MESSAGE_MAP()


// CStart 消息处理程序
