// CCondenseDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CCondenseDlg.h"
#include "afxdialogex.h"
#include <winternl.h>


// CCondenseDlg 对话框

//定义必要的函数指针
typedef NTSTATUS(WINAPI* typedef_RtlGetCompressionWorkSpaceSize)(
	_In_  USHORT CompressionFormatAndEngine,
	_Out_ PULONG CompressBufferWorkSpaceSize,
	_Out_ PULONG CompressFragmentWorkSpaceSize
	);

typedef NTSTATUS(WINAPI *typedef_RtlCompressBuffer)(
	_In_  USHORT CompressionFormatAndEngine,
	_In_  PUCHAR UncompressedBuffer,
	_In_  ULONG  UncompressedBufferSize,
	_Out_ PUCHAR CompressedBuffer,
	_In_  ULONG  CompressedBufferSize,
	_In_  ULONG  UncompressedChunkSize,
	_Out_ PULONG FinalCompressedSize,
	_In_  PVOID  WorkSpace
	);

typedef NTSTATUS(WINAPI *typedef_RtlDecompressBuffer)(
	_In_  USHORT CompressionFormat,
	_Out_ PUCHAR UncompressedBuffer,
	_In_  ULONG  UncompressedBufferSize,
	_In_  PUCHAR CompressedBuffer,
	_In_  ULONG  CompressedBufferSize,
	_Out_ PULONG FinalUncompressedSize
	);


IMPLEMENT_DYNAMIC(CCondenseDlg, CDialogEx)

CCondenseDlg::CCondenseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONDENSE_DIALOG, pParent)
	, m_Edit_Data(_T(""))
	, m_Tip(_T(""))
	, m_Edit_Path(_T("文件解压缩需拖拽文件至此处"))
{

}

CCondenseDlg::~CCondenseDlg()
{
}

void CCondenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_Data);
	DDX_Text(pDX, IDC_EDIT2, m_Tip);
	DDX_Text(pDX, IDC_EDIT3, m_Edit_Path);
}


BEGIN_MESSAGE_MAP(CCondenseDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCondenseDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCondenseDlg::OnBnClickedButton2)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON8, &CCondenseDlg::OnBnClickedButton8)
END_MESSAGE_MAP()

// CCondenseDlg 消息处理程序



//数据解压缩按钮
void CCondenseDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	//检测数据是否为空
	if (m_Edit_Data == "")
	{
		m_Tip = "数据不能为空";
		UpdateData(FALSE);
		return;
	}

	BYTE *pCompressData = NULL;		//压缩后的数据
	DWORD dwCompressDataLength = 0;	//压缩后数据的长度
	BYTE* pUnCompressData = NULL;	//压缩前的数据
	DWORD dwUnCompressDataLength;	//压缩前的数据长度

	USES_CONVERSION;
	pUnCompressData = (BYTE*)W2A(m_Edit_Data);
	dwUnCompressDataLength = ::lstrlenA((LPCSTR)pUnCompressData);

	//压缩数据
	DataCompress(pUnCompressData, dwUnCompressDataLength, &pCompressData, &dwCompressDataLength);
	

	CString temp;
	//清空控件上显示的内容
	m_Edit_Data = "";
	m_Tip = "";

	//显示原数据
	m_Edit_Data += "原数据：\r\n";
	temp = ByteToCString(pUnCompressData, dwUnCompressDataLength);
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";

	//显示压缩后的数据
	m_Edit_Data += "压缩后的数据为：\r\n";
	temp = ByteToCString(pCompressData, dwCompressDataLength);
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";


	//解压数据
	UncompressData(pCompressData, dwCompressDataLength, &pUnCompressData, &dwUnCompressDataLength);

	//显示解压后的数据
	m_Edit_Data += "解压后的数据为：\r\n";
	temp = ByteToCString(pUnCompressData, dwUnCompressDataLength);
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";

	//显示到控件
	UpdateData(FALSE);

}

//文件压缩按钮
void CCondenseDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	//检查文件是否存在
	if (GetFileAttributes(m_Edit_Path) == INVALID_FILE_ATTRIBUTES)
	{
		m_Tip = "文件不存在!请重试！";
		return;
	}

	BOOL bRet = FALSE;
	BYTE* pCompressData = NULL;
	DWORD dwCompressDataSize = 0;


	//清空控件上显示的内容
	m_Tip = "";

	USES_CONVERSION;
	char* pszFilePath = W2A(m_Edit_Path);

	//将文件压缩为数据
	bRet = Zlib_CompressData(pszFilePath, &pCompressData, &dwCompressDataSize);
	if (bRet)
	{
		m_Tip += "将文件压缩为数据成功\r\n";
	}
	else
	{
		m_Tip += "将文件压缩为数据失败\r\n";
		return;
	}

	//获取源文件名
	PathStripPathA(pszFilePath);
	//去掉文件名后缀
	(strrchr(pszFilePath, '.'))[0] = 0;
	//加上我们自己的后缀
	char pszDestFileName[MAX_PATH];
	sprintf_s(pszDestFileName, "%s.%s", pszFilePath, "XiaoBai");

	//保存压缩数据为文件
	bRet = SaveToFile(pszDestFileName, pCompressData, dwCompressDataSize);
	if (bRet == FALSE)
	{
		m_Tip += "保存压缩数据为 ";
		m_Tip += pszDestFileName;
		m_Tip += "文件失败\r\n";
		return;
	}
	else
	{
		m_Tip += "保存压缩数据为 ";
		m_Tip += pszDestFileName;
		m_Tip += "文件成功\r\n";
	}

	UpdateData(FALSE);
}

//文件解压缩按钮
void CCondenseDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	//检查后缀是否为.XiaoBai
	LPTSTR pszExtension = PathFindExtension(m_Edit_Path);
	if (lstrcmp(pszExtension, L".XiaoBai") != 0)
	{
		MessageBox(_T("请先拖拽有效的压缩文件，后缀为XiaoBai"));
		return;
	}

	//检查文件是否存在
	if (GetFileAttributes(m_Edit_Path) == INVALID_FILE_ATTRIBUTES)
	{
		m_Tip = "文件不存在!请重试！";
		return;
	}

	BOOL bRet = FALSE;
	BYTE* pUnCompressData = NULL;
	DWORD dwUnCompressDataSize = 0;

	//清空控件上显示的内容
	m_Tip = "";

	USES_CONVERSION;
	char* pszFilePath = W2A(m_Edit_Path);

	//将文件解压缩为数据
	bRet = Zlib_UnCompressData(pszFilePath, &pUnCompressData, &dwUnCompressDataSize);
	if (bRet)
	{
		m_Tip += "将文件解压缩为数据成功\r\n";
	}
	else
	{
		m_Tip += "将文件解压缩为数据失败\r\n";
		return;
	}

	//获取源文件名
	PathStripPathA(pszFilePath);
	//去掉文件名后缀
	(strrchr(pszFilePath, '.'))[0] = 0;
	//加上我们自己的后缀
	char pszDestFileName[MAX_PATH];
	sprintf_s(pszDestFileName, "%s_%s.%s", pszFilePath, "XB","exe");

	//保存解压缩数据为文件
	bRet = SaveToFile(pszDestFileName, pUnCompressData, dwUnCompressDataSize);
	if (bRet == FALSE)
	{
		m_Tip += "保存解压缩数据为 ";
		m_Tip += pszDestFileName;
		m_Tip += "文件失败\r\n";
		return;
	}
	else
	{
		m_Tip += "保存解压缩数据为 ";
		m_Tip += pszDestFileName;
		m_Tip += "文件成功\r\n";
	}

	UpdateData(FALSE);
}

BOOL CCondenseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);       // 0x0049 == WM_COPYGLOBALDATA

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCondenseDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取文件路径
	TCHAR szPath[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);

	//显示到控件上
	m_Edit_Path = szPath;
	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}

//把BYTE* 转换为十六进制显示的CString
CString CCondenseDlg::ByteToCString(BYTE* str, DWORD dwLength)
{
	//拷贝压缩之后的数据到数组
	BYTE tempStr[4096] = { 0 };
	memcpy(tempStr, str, dwLength);

	//将解压缩的数据转为十六进制 显示到界面
	CString cStr;
	CString data;
	DWORD i = 0;

	for (i = 0; i < dwLength; i++)
	{
		cStr.Format(L"%02X", tempStr[i]);
		data += cStr;
	}
	return data;
}

//数据压缩
BOOL CCondenseDlg::DataCompress(BYTE* pUncompressData, DWORD dwUncompressDataLength, BYTE** ppCompressData, DWORD* pdwCompressDataLength)
{
	BOOL bRet = FALSE;
	NTSTATUS status = 0;
	HMODULE hModule = NULL;

	//定义函数指针变量
	typedef_RtlGetCompressionWorkSpaceSize RtlGetCompressionWorkSpaceSize = NULL;
	typedef_RtlCompressBuffer RtlCompressBuffer = NULL;

	DWORD dwWorkSpaceSize = 0;
	DWORD dwFragmentWorkSpaceSize = 0;
	BYTE* pWorkSpace = NULL;
	BYTE* pCompressData = NULL;
	DWORD dwCompressDataLength = 4096;
	DWORD dwFinalCompressSize = 0;

	//加载ntdll.dll
	hModule = LoadLibrary(_T("ntdll.dll"));
	if (hModule == NULL)
	{
		m_Tip += _T("LoadLibrary Error\r\n");
		return bRet;
	}

	//获取函数地址
	RtlGetCompressionWorkSpaceSize = (typedef_RtlGetCompressionWorkSpaceSize)::GetProcAddress(hModule, "RtlGetCompressionWorkSpaceSize");
	if (NULL == RtlGetCompressionWorkSpaceSize)
	{
		m_Tip += _T("GetProcAddress Error\r\n");
		FreeLibrary(hModule);
		return bRet;
	}

	RtlCompressBuffer = (typedef_RtlCompressBuffer)::GetProcAddress(hModule, "RtlCompressBuffer");
	if (NULL == RtlCompressBuffer)
	{
		m_Tip += _T("GetProcAddress Error\r\n");
		FreeLibrary(hModule);
		return bRet;
	}

	//获取WorkSpace大小
	status = RtlGetCompressionWorkSpaceSize(COMPRESSION_FORMAT_LZNT1 | COMPRESSION_ENGINE_STANDARD,
		&dwWorkSpaceSize, &dwFragmentWorkSpaceSize);
	if (0 != status)
	{
		m_Tip += _T("RtlGetCompressionWorkSpaceSize Error\r\n");
		FreeLibrary(hModule);
		return bRet;
	}

	//申请动态内存
	pWorkSpace = new BYTE[dwWorkSpaceSize]{ 0 };
	if (pWorkSpace == NULL)
	{
		m_Tip += _T("new Error\r\n");
		FreeLibrary(hModule);
		return bRet;
	}

	while (TRUE)
	{
		//申请动态内存存储压缩后的数据
		pCompressData = new BYTE[dwCompressDataLength];
		if (pCompressData == NULL)
		{
			m_Tip += _T("new Error\r\n");
			delete[] pWorkSpace;
			FreeLibrary(hModule);
			return bRet;
		}

		//开始压缩数据 
		RtlCompressBuffer(COMPRESSION_FORMAT_LZNT1, pUncompressData, dwUncompressDataLength,
			(PUCHAR)pCompressData, dwCompressDataLength, 4096, &dwFinalCompressSize, (PVOID)pWorkSpace);

		//如果压缩数据的缓冲区小于最终的压缩大小 说明压缩失败 需要重新压缩
		if (dwCompressDataLength < dwFinalCompressSize)
		{
			//释放内存
			if (pCompressData)
			{
				delete[] pCompressData;
			}
			dwCompressDataLength = dwFinalCompressSize;
		}
		else
		{
			//否则说明压缩成功 退出循环
			break;
		}
	}

	//返回压缩后的数据和长度
	*ppCompressData = pCompressData;
	*pdwCompressDataLength = dwFinalCompressSize;
	bRet = TRUE;

	//释放资源
	if (pWorkSpace)
	{
		delete[] pWorkSpace;
	}
	if (hModule)
	{
		FreeLibrary(hModule);
	}
	return bRet;
}



BOOL CCondenseDlg::UncompressData(BYTE *pCompressData, DWORD dwCompressDataLength, BYTE **ppUncompressData, DWORD *pdwUncompressDataLength)
{
	BOOL bRet = FALSE;
	HMODULE hModule = NULL;
	typedef_RtlDecompressBuffer RtlDecompressBuffer = NULL;
	BYTE* pUncompressData = NULL;
	DWORD dwUncompressDataLength = 4096;
	DWORD dwFinalUncompressSize = 0;

	// 加载 ntdll.dll 
	hModule = ::LoadLibrary(_T("ntdll.dll"));
	if (NULL == hModule)
	{
		m_Tip += _T("LoadLibrary Error\r\n");
		return bRet;
	}

	// 获取 RtlDecompressBuffer 函数地址
	RtlDecompressBuffer = (typedef_RtlDecompressBuffer)::GetProcAddress(hModule, "RtlDecompressBuffer");
	if (NULL == RtlDecompressBuffer)
	{
		m_Tip += _T("GetProcAddress Error\r\n");
		FreeLibrary(hModule);
		return bRet;
	}

	while (TRUE)
	{
		// 申请动态内存
		pUncompressData = new BYTE[dwUncompressDataLength];
		if (NULL == pUncompressData)
		{
			m_Tip += _T("new Error\r\n");
			FreeLibrary(hModule);
			return bRet;
		}
		::RtlZeroMemory(pUncompressData, dwUncompressDataLength);

		// 调用RtlCompressBuffer压缩数据
		RtlDecompressBuffer(COMPRESSION_FORMAT_LZNT1, pUncompressData, dwUncompressDataLength, pCompressData, dwCompressDataLength, &dwFinalUncompressSize);
		if (dwUncompressDataLength < dwFinalUncompressSize)
		{
			// 释放内存
			if (pUncompressData)
			{
				delete[] pUncompressData;
				pUncompressData = NULL;
			}
			dwUncompressDataLength = dwFinalUncompressSize;
		}
		else
		{
			break;
		}
	}

	//返回解压后的数据和长度
	*ppUncompressData = pUncompressData;
	*pdwUncompressDataLength = dwFinalUncompressSize;
	bRet = TRUE;

	//释放资源
	if (hModule)
	{
		::FreeLibrary(hModule);
	}

	return bRet;
}

//将文件压缩为数据
BOOL CCondenseDlg::Zlib_CompressData(char* pszCompressFileName, BYTE** ppCompressData, DWORD* pdwCompressDataSize)
{
	//注意可能出现压缩后的文件比压缩前的文件大的现象
	//打开文件 获取数据
	HANDLE hFile = CreateFileA(pszCompressFileName, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		m_Tip += "CreateFileA Error\r\n";
		return FALSE;
	}

	//获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (MAX_SRC_FILE_SIZE < dwFileSize)
	{
		m_Tip += "单个文件大小限制为100M！\r\n";
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwDestDataSize = dwFileSize;

	//申请空间存放源文件
	BYTE* pSrcData = new BYTE[dwFileSize]{ 0 };
	if (pSrcData == NULL)
	{
		m_Tip += "new Error\r\n";
		CloseHandle(hFile);
		return FALSE;
	}

	//申请空间存放压缩后的目标文件
	BYTE* pDestData = new BYTE[dwDestDataSize]{ 0 };
	if (pDestData == NULL)
	{
		delete[] pSrcData;
		pSrcData = NULL;
		m_Tip += "new Error\r\n";
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwRet = 0;

	//读取源文件到内存
	ReadFile(hFile, pSrcData, dwFileSize, &dwRet, NULL);
	if ((0 >= dwRet) || dwRet != dwFileSize)
	{
		delete[] pSrcData;
		pSrcData = NULL;
		delete[] pDestData;
		pDestData = NULL;
		m_Tip += "ReadFile Error\r\n";
		CloseHandle(hFile);
		return FALSE;
	}

	//开始压缩数据
	int iRet = 0;

	do
	{
		iRet = compress(pDestData, &dwDestDataSize, pSrcData, dwFileSize);
		if (0 == iRet)
		{
			//成功
			break;
		}
		else if (-5 == iRet)
		{
			//输出缓冲区不够大 以100KB大小递增 
			delete[] pDestData;
			dwDestDataSize = dwDestDataSize + (100 * 1024);
			pDestData = new BYTE[dwDestDataSize];
			if (pDestData == NULL)
			{
				delete[] pSrcData;
				pSrcData = NULL;
				delete[] pDestData;
				pDestData = NULL;
				m_Tip += "输出缓冲区不够大 Error\r\n";
				CloseHandle(hFile);
				return FALSE;
			}
		}
		else
		{
			//没有足够的内存或者其他情况
			delete[] pSrcData;
			pSrcData = NULL;
			delete[] pDestData;
			pDestData = NULL;
			m_Tip += "没有足够的内存或者其他情况 Error\r\n";
			CloseHandle(hFile);
			return FALSE;
		}
	} while (TRUE);

	//返回数据
	*ppCompressData = pDestData;
	*pdwCompressDataSize = dwDestDataSize;

	//释放内存
	delete[] pSrcData;
	pSrcData = NULL;
	CloseHandle(hFile);
	return TRUE;
}

//将数据保存为文件
BOOL CCondenseDlg::SaveToFile(char * pszFileName, BYTE * pData, DWORD dwDataSize)
{
	//创建文件
	HANDLE hFile = CreateFileA(pszFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		m_Tip += "CreateFileA Error\r\n";
		return FALSE;
	}

	//写入数据
	DWORD dwRet = 0;
	WriteFile(hFile, pData, dwDataSize, &dwRet, NULL);
	if (dwRet != dwDataSize)
	{
		m_Tip += "WriteFile Error\r\n";
		CloseHandle(hFile);
		return FALSE;
	}

	//关闭句柄
	CloseHandle(hFile);
	return TRUE;
}

//将文件解压缩为数据
BOOL CCondenseDlg::Zlib_UnCompressData(char * pszUncompressFileName, BYTE ** ppUnCompressData, DWORD* pdwUnCompressDataSize)
{
	//打开文件并获取数据
	HANDLE hFile = CreateFileA(pszUncompressFileName, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		m_Tip += "CreateFileA Error\r\n";
		return FALSE;
	}

	//获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwDestDataSize = MAX_SRC_FILE_SIZE;	//目标文件大小

	//根据源文件大小申请空间
	BYTE* pSrcData = new BYTE[dwFileSize]{ 0 };
	if (pSrcData == NULL)
	{
		m_Tip += "new Error\r\n";
		CloseHandle(hFile);
		return FALSE;
	}

	//根据目标文件大小申请空间
	BYTE* pDestData = new BYTE[dwDestDataSize]{ 0 };
	if (pDestData == NULL)
	{
		delete[] pSrcData;
		pSrcData = NULL;
		m_Tip += "new Error\r\n";
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwRet = 0;

	//读取需要解压缩的文件的到内存
	ReadFile(hFile, pSrcData, dwFileSize, &dwRet, NULL);
	if ((0 >= dwRet) || dwRet != dwFileSize)
	{
		delete[] pDestData;
		pDestData = NULL;
		delete[] pSrcData;
		pSrcData = NULL;
		m_Tip += "ReadFile Error\r\n";
		::CloseHandle(hFile);
		return FALSE;
	}

	//解压缩数据
	int iRet = 0;
	do
	{
		iRet = uncompress(pDestData, &dwDestDataSize, pSrcData, dwFileSize);
		if (0 == iRet)
		{
			//成功
			break;
		}
		else if (-5 == iRet)
		{
			//输出缓冲区不够大 以100KB大小递增
			delete[]pDestData;
			pDestData = NULL;
			dwDestDataSize = dwDestDataSize + (100 * 1024);
			pDestData = new BYTE[dwDestDataSize];
			if (NULL == pDestData)
			{
				delete[] pSrcData;
				pSrcData = NULL;
				delete[] pDestData;
				pDestData = NULL;
				m_Tip += "输出缓冲区不够大 Error\r\n";
				::CloseHandle(hFile);
				return FALSE;
			}
		}
		else
		{
			//没有足够的内存 或其他情况
			delete[] pDestData;
			pDestData = NULL;
			delete[] pSrcData;
			pSrcData = NULL;
			m_Tip += "没有足够的内存 或其他情况 Error\r\n";
			::CloseHandle(hFile);
			return FALSE;
		}
	} while (TRUE);

	//返回数据
	*ppUnCompressData = pDestData;
	*pdwUnCompressDataSize = dwDestDataSize;

	//释放内存
	delete[] pSrcData;
	pSrcData = NULL;
	::CloseHandle(hFile);
	return TRUE;
}


