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
	, m_Edit_Basic(_T("防多开处于开启状态"))
{
	m_PreMulOpenTipMsg = _T("防多开处于开启状态\r\n");
}

CBasicsDlg::~CBasicsDlg()
{
}

void CBasicsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BASIC, m_Edit_Basic);
}


BEGIN_MESSAGE_MAP(CBasicsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBasicsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBasicsDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CBasicsDlg 消息处理程序

//单击防多开按钮-开启/关闭防多开(默认是开启的)
void CBasicsDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	static BOOL BnClick = TRUE;	//防多开被点击次数为奇数时为FALSE,
								//为偶数时为TRUE
	if (BnClick)
	{
		CloseHandle(g_hMutex);	//关闭互斥体对象
		BnClick = FALSE;
		//更新防多开提示信息
		m_PreMulOpenTipMsg = _T("防多开处于关闭状态\r\n");
	}
	else
	{
		//创建互斥体
		g_hMutex = CreateMutex(NULL, FALSE, _T("HackTool"));
		BnClick = TRUE;
		//更新防多开提示信息
		m_PreMulOpenTipMsg = _T("防多开处于开启状态\r\n");
	}
	//更新提示信息并显示
	m_Edit_Basic = m_PreMulOpenTipMsg + m_FreeResTipMsg;
	UpdateData(FALSE);
}

//单击释放资源按钮-释放以资源形式插入程序的文件
void CBasicsDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//释放资源并以文件形式保存
	FreeMyResource(IDR_MYRES2, (char*)L"MYRES", "MyHookDll");
	FreeMyResource(IDR_MYRES3, (char*)L"MYRES", "RemoteThreadDll");

	//更新提示信息并显示
	m_Edit_Basic = m_PreMulOpenTipMsg + m_FreeResTipMsg;
	UpdateData(FALSE);
}


BOOL CBasicsDlg::FreeMyResource(UINT uiResourceName, char* lpszResourceType, char* lpszSaveFileName)
{
	//获取指定模块里的资源
	HRSRC hRsrc = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(uiResourceName), (LPCWSTR)lpszResourceType);
	if (NULL == hRsrc)
	{
		m_FreeResTipMsg += _T("获取资源")+ CString(lpszSaveFileName)+_T("失败\r\n");
		return FALSE;
	}
	else
	{
		m_FreeResTipMsg += _T("获取资源") + CString(lpszSaveFileName) + _T("成功\r\n");
	}
	//获取资源大小
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (dwSize <= 0)
	{
		m_FreeResTipMsg += _T("获取") + CString(lpszSaveFileName) + _T("资源大小失败\r\n");
		return FALSE;
	}
	else
	{
		m_FreeResTipMsg += _T("获取") + CString(lpszSaveFileName) + _T("资源大小成功\r\n");
	}
	//将资源加载到内存里
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		m_FreeResTipMsg += _T("资源") + CString(lpszSaveFileName) + _T("加载到内存失败\r\n");
		return FALSE;
	}
	else
	{
		m_FreeResTipMsg += _T("资源") + CString(lpszSaveFileName) + _T("加载到内存成功\r\n");
	}
	//锁定资源
	LPVOID lpVoid = LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		m_FreeResTipMsg += _T("资源") + CString(lpszSaveFileName) + _T("锁定失败\r\n");
		return FALSE;
	}
	else
	{
		m_FreeResTipMsg += _T("资源") + CString(lpszSaveFileName) + _T("锁定成功\r\n");
	}
	//保存资源为文件
	FILE* fp = NULL;
	fopen_s(&fp, lpszSaveFileName,"wb+");
	if (NULL == fp)
	{
		m_FreeResTipMsg += _T("创建文件") + CString(lpszSaveFileName) + _T("失败\r\n");
		return FALSE;
	}
	DWORD res = fwrite(lpVoid, sizeof(char), dwSize, fp);
	if (res != dwSize)
	{
		m_FreeResTipMsg += _T("资源") + CString(lpszSaveFileName) + _T("保存为文件时有数据丢失\r\n");
		fclose(fp);
		return FALSE;
	}
	else
	{
		m_FreeResTipMsg += _T("资源") + CString(lpszSaveFileName) + _T("成功保存为文件\r\n");
		fclose(fp);
		return TRUE;
	}

}