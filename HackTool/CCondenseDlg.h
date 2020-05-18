#pragma once

//*************************************************
//         ZLIB压缩库的头文件和静态库
//*************************************************
#include "zlib\\zconf.h"
#include "zlib\\zlib.h"

#ifdef _DEBUG
	#ifdef _WIN64
		#pragma comment(lib, "zlib\\x64\\debug\\zlibstat.lib")
	#else
		#pragma comment(lib,"zlib\\x86\\debug\\zlibstat.lib")
	#endif
#else
	#ifdef _WIN64
		#pragma comment(lib, "zlib\\x64\\release\\zlibstat.lib")
	#else
		#pragma comment(lib, "zlib\\x86\\release\\zlibstat.lib")
	#endif
#endif
//************************************************* 

#define MAX_SRC_FILE_SIZE (100*1024*1024)			//单个文件限制大小为100M

// CCondenseDlg 对话框

class CCondenseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCondenseDlg)

public:
	CCondenseDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCondenseDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONDENSE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//************************************
	// 函数名：CCondenseDlg::DataCompress
	// 返回类型：BOOL
	// 功能:	数据压缩
	// 参数1：BYTE* pUncompressData 未压缩的数据
	// 参数2：DWORD dwUncompressDataLength	未压缩的数据的数据大小
	// 参数3：BYTE** ppCompressData	压缩后的数据
	// 参数4：DWORD* pdwCompressDataLength	压缩后的数据大小
	//************************************
	BOOL DataCompress(BYTE* pUncompressData, DWORD dwUncompressDataLength, BYTE** ppCompressData, DWORD* pdwCompressDataLength);

	//************************************
	// 函数名：CCondenseDlg::UncompressData
	// 返回类型：BOOL
	// 功能:	数据解压缩
	// 参数1：BYTE *pCompressData 压缩的数据
	// 参数2：DWORD dwCompressDataLength 压缩的数据大小
	// 参数3：BYTE **ppUncompressData 解压缩的数据
	// 参数4：DWORD *pdwUncompressDataLength 解压缩的数据大小
	//************************************
	BOOL UncompressData(BYTE *pCompressData, DWORD dwCompressDataLength, BYTE **ppUncompressData, DWORD *pdwUncompressDataLength);

	//************************************
	// 函数名：CCondenseDlg::ByteToCString
	// 返回类型：CString
	// 功能:	把BYTE* 转换为十六进制显示的CString
	// 参数1：BYTE* str	需要转换的BYTE*
	// 参数2：DWORD dwLength	需要转换的BYTE*的长度
	//************************************
	CString ByteToCString(BYTE* str, DWORD dwLength);

	//************************************
	// 函数名：CCondenseDlg::Zlib_CompressData
	// 返回类型：BOOL
	// 功能:	将文件压缩为数据
	// 参数1：char* pszCompressFileName 需要压缩的文件完整路径
	// 参数2：BYTE** ppCompressData 压缩后的数据
	// 参数3：DWORD* pdwCompressDataSize 压缩后的数据大小
	//************************************
	BOOL Zlib_CompressData(char* pszCompressFileName, BYTE** ppCompressData, DWORD* pdwCompressDataSize);

	//************************************
	// 函数名：CCondenseDlg::SaveToFile
	// 返回类型：BOOL
	// 功能:	将数据保存为文件
	// 参数1：char * pszFileName 文件名
	// 参数2：BYTE * pData 数据
	// 参数3：DWORD dwDataSize 数据大小
	//************************************
	BOOL SaveToFile(char * pszFileName, BYTE * pData, DWORD dwDataSize);

	//************************************
	// 函数名：CCondenseDlg::Zlib_UnCompressData
	// 返回类型：BOOL
	// 功能:	将文件解压缩为数据
	// 参数1：char * pszUncompressFileName 需要解压缩的文件名
	// 参数2：BYTE ** ppUnCompressData 解压缩后的数据
	// 参数3：DWORD* pdwUnCompressDataSize 解压缩后的数据大小
	//************************************
	BOOL Zlib_UnCompressData(char * pszUncompressFileName, BYTE ** ppUnCompressData, DWORD* pdwUnCompressDataSize);

public:
	CString m_Edit_Data;
	CString m_Tip;
	CString m_Edit_Path;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnBnClickedButton8();
};
