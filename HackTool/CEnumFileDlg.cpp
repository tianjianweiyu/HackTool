// CEnumFileDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CEnumFileDlg.h"
#include "afxdialogex.h"
#include "CDiscDlg.h"
#include "CFileAttrDlg.h"


// CEnumFileDlg 对话框

IMPLEMENT_DYNAMIC(CEnumFileDlg, CDialogEx)

CEnumFileDlg::CEnumFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENUMFILE_DIALOG, pParent)
{

}

CEnumFileDlg::~CEnumFileDlg()
{
}

void CEnumFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Combox);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CEnumFileDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CEnumFileDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CEnumFileDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CEnumFileDlg::OnCbnSelchangeCombo1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CEnumFileDlg::OnNMRClickList1)
	ON_COMMAND(ID_32772, &CEnumFileDlg::OnPopFileAttr)
END_MESSAGE_MAP()


// CEnumFileDlg 消息处理程序


BOOL CEnumFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化Combox
	//初始化下拉列表相关信息 获取盘符
	TCHAR NameBuffer[MAX_PATH] = { 0 };

	//获取所有逻辑驱动器，放到NameBuffer里
	GetLogicalDriveStrings(MAX_PATH, NameBuffer);

	//循环所有字符，每个字符以\0结尾，循环一次只能拿到一个字符
	//循环完了之后需要加上字符长度
	for (int i = 0; NameBuffer[i] != 0; i++)
	{
		//显示到Combox
		m_Combox.AddString(&NameBuffer[i]);
		i += _tcslen(&NameBuffer[i]);
	}

	//设置Combox的默认选项
	m_Combox.SetCurSel(0);

	//初始化ListCtrl
	//设置ListCtrl控件扩展风格
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	m_ListCtrl.InsertColumn(0, _T("名称"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("创建时间"), 0, 200);
	m_ListCtrl.InsertColumn(2, _T("类型"), 0, 100);
	m_ListCtrl.InsertColumn(3, _T("大小"), 0, 100);

	//遍历文件(默认遍历C盘下的文件)
	BOOL bRet = EnumFiles(_T("C:\\"));
	if (bRet == FALSE)
	{
		MessageBox(_T("文件遍历失败！"));
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//遍历文件
BOOL CEnumFileDlg::EnumFiles(TCHAR *pszDirectory)
{
	m_FilePath = pszDirectory;
	
	//清空ListCtrl
	m_ListCtrl.DeleteAllItems();

	// 搜索指定类型的文件
	CString temp;
	WIN32_FIND_DATA FileData = { 0 };

	// 构造搜索文件类型字符串, *.*表示搜索所有文件类型
	temp.Format(_T("%s\\*.*"),CString(pszDirectory));

	// 搜索第一个文件
	HANDLE hFile = ::FindFirstFile(temp, &FileData);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(_T("FindFirstFile Error"));
		return FALSE;
	}

	int index = 0;

	do
	{
		//// 要过滤掉当前目录"." 和上一层目录"..", 否则会进入死循环遍历
		//if (lstrcmp(FileData.cFileName, L".") == 0 || lstrcmp(FileData.cFileName, L"..") == 0)
		//{
		//	continue;
		//}

		//把文件名添加到行
		m_ListCtrl.InsertItem(index, (LPCTSTR)FileData.cFileName);

		//将文件时间转为系统时间
		SYSTEMTIME SystemTime = { 0 };
		FileTimeToSystemTime(&FileData.ftCreationTime, &SystemTime);
		temp.Format(_T("%4d/%02d/%02d/ %02d:%02d:%02d"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
			SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
		//把创建时间添加到行
		m_ListCtrl.SetItemText(index, 1,temp);

		temp = ' ';
		//判断文件属性
		if (FileData.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)
		{
			temp += "隐藏";
		}
		if (FileData.dwFileAttributes&FILE_ATTRIBUTE_READONLY)
		{
			temp += "只读";
		}
		if (FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			temp += "目录";
		}
		else
		{
			temp += "文件";
		}
		//把类型添加到行
		m_ListCtrl.SetItemText(index, 2, temp);

		if (FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			temp = "-";
		}
		else
		{
			if (FileData.nFileSizeLow > 1073741824)
			{
				temp.Format(_T("%.2lfGB"), FileData.nFileSizeLow / 1024.0 / 1024.0 / 1024.0);
			}
			else if (FileData.nFileSizeLow > 1048576)
			{
				temp.Format(_T("%.2lfMB"), FileData.nFileSizeLow / 1024.0 / 1024.0);
			}
			else
			{
				temp.Format(_T("%.2lfKB"), FileData.nFileSizeLow / 1024.0);
			}
		}
		//把大小添加到行
		m_ListCtrl.SetItemText(index, 3, temp);

		index++;

		// 搜索下一个文件
	} while (::FindNextFile(hFile, &FileData));

	// 关闭文件句柄
	::FindClose(hFile);

	return TRUE;
}

//响应List控件鼠标左键双击事件
void CEnumFileDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if (GetFileAttributes(m_FilePath)&FILE_ATTRIBUTE_DIRECTORY)
	{
		//获取选中的行
		int i = m_ListCtrl.GetSelectionMark();
		//获取对应的文件名
		CString FileName = m_ListCtrl.GetItemText(i, 0);
		m_FilePath.Append(_T("\\"));
		m_FilePath.Append(FileName);
		EnumFiles(m_FilePath.GetBuffer());
	}
	*pResult = 0;
}

//查看按钮
void CEnumFileDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR temp[20] = { 0 };
	//将下拉框的字符串以构造函数的形式传给磁盘查看对话框
	m_Combox.GetWindowText(temp, 20);
	CDiscDlg MyDiscDlg(temp);
	MyDiscDlg.DoModal();
}

//响应切换下拉框消息
void CEnumFileDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR temp[MAX_PATH] = { 0 };
	//获取Combox的选中内容
	m_Combox.GetLBText(m_Combox.GetCurSel(), temp);
	//根据选中的盘符遍历文件
	EnumFiles(temp);
}

//响应List控件的右键消息
void CEnumFileDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	CMenu* pmenu = menu.GetSubMenu(0);
	//获取当前光标的位置
	CPoint point1;
	GetCursorPos(&point1);
	//在指定位置显示弹出菜单
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);

	*pResult = 0;
}

//响应属性菜单的点击
void CEnumFileDlg::OnPopFileAttr()
{
	// TODO: 在此添加命令处理程序代码
	//获取选中的行号
	int i = m_ListCtrl.GetSelectionMark();
	//根据选中的行号获取对应的文件名
	CString FileName = m_ListCtrl.GetItemText(i, 0);
	//拼接出完整路径
	CString temp = m_FilePath + _T("\\") + FileName;
	//弹出对话框，把文件完整路径通过构造函数的参数传过去
	CFileAttrDlg MyFileAttrDlg(temp.GetBuffer());
	MyFileAttrDlg.DoModal();
}
