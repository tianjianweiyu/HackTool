// CEncryptionDlg.cpp: 实现文件
//

#include "pch.h"
#include "HackTool.h"
#include "CEncryptionDlg.h"
#include "afxdialogex.h"


// CEncryptionDlg 对话框

IMPLEMENT_DYNAMIC(CEncryptionDlg, CDialogEx)

CEncryptionDlg::CEncryptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENCRYPTION_DIALOG, pParent)
	, m_Edit_Path(_T(""))
	, m_Edit_Data(_T(""))
	, m_Edit_Aes(_T("输入AES密码"))
{

}

CEncryptionDlg::~CEncryptionDlg()
{
}

void CEncryptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_Path);
	DDX_Text(pDX, IDC_EDIT2, m_Edit_Data);
	DDX_Text(pDX, IDC_EDIT3, m_Edit_Aes);
}


BEGIN_MESSAGE_MAP(CEncryptionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEncryptionDlg::OnBnClickedButton1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CEncryptionDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON8, &CEncryptionDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CEncryptionDlg 消息处理程序

//hash按钮
void CEncryptionDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	//检测文件是否存在
	if (GetFileAttributes(m_Edit_Path) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBox(_T("文件不存在！"));
		return;
	}

	//清空数据
	m_Edit_Data = "";

	BYTE* pData = NULL;
	DWORD dwDataLength = 0;
	BYTE* pHashData = NULL;
	DWORD dwHashLength = 0;

	//读取文件数据
	USES_CONVERSION;
	GetFileData(W2A(m_Edit_Path), &pData, &dwDataLength);

	//计算MD5值
	CalculateHash(pData, dwDataLength, CALG_MD5, &pHashData, &dwHashLength);
	//显示到界面
	CString temp;
	temp = ByteToCString(pHashData, dwHashLength);
	m_Edit_Data += "MD5:\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n\r\n";

	//计算SHA1值
	CalculateHash(pData, dwDataLength, CALG_SHA1, &pHashData, &dwHashLength);
	//显示到界面
	temp = ByteToCString(pHashData, dwHashLength);
	m_Edit_Data += "SHA1:\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n\r\n";

	//计算SHA256值
	CalculateHash(pData, dwDataLength, CALG_SHA_256, &pHashData, &dwHashLength);
	//显示到界面
	temp = ByteToCString(pHashData, dwHashLength);
	m_Edit_Data += "SHA256:\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n\r\n";

	UpdateData(FALSE);

}

//AES按钮
void CEncryptionDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	//将CString转为BYTE*
	USES_CONVERSION;
	BYTE pData[4096] = { 0 };

	DWORD dwDataLength = lstrlenA((LPCSTR)W2A(m_Edit_Data))+1;
	memcpy(pData, W2A(m_Edit_Data), dwDataLength);
	
	//清空数据
	m_Edit_Data = "";

	//显示原数据
	CString temp;
	temp = ByteToCString(pData, dwDataLength);
	m_Edit_Data += "原数据：\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";

	DWORD dwBufferLength = 4096;
	BYTE* pPassword = (BYTE*)W2A(m_Edit_Aes);
	DWORD dwPasswordLength = lstrlenA((LPCSTR)pPassword);

	//AES加密
	AesEncrypt(pPassword, dwPasswordLength, pData, dwDataLength, dwBufferLength);
	//显示加密的数据
	temp = ByteToCString(pData, dwDataLength);
	m_Edit_Data += "AES加密后的数据：\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";

	//AES解密
	AesDecrypt(pPassword, dwPasswordLength, pData, dwDataLength);
	//显示解密的数据
	temp = ByteToCString(pData, dwDataLength);
	m_Edit_Data += "AES解密后的数据：\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";

	UpdateData(FALSE);

	return;
}

//RSA按钮
void CEncryptionDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	//将CString转为BYTE*
	USES_CONVERSION;
	BYTE pData[4096] = { 0 };		//需要加解密的数据

	//需要加解密的数据长度
	DWORD dwDataLength = lstrlenA((LPCSTR)W2A(m_Edit_Data)) + 1;
	memcpy(pData, W2A(m_Edit_Data), dwDataLength);

	//清空数据
	m_Edit_Data = "";

	//显示原数据
	CString temp;
	temp = ByteToCString(pData, dwDataLength);
	m_Edit_Data += "原数据：\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";

	BYTE* pPublicKey = NULL;		//公钥
	DWORD dwPublicKeyLength = 0;	//公钥长度
	BYTE* pPrivateKey = NULL;		//私钥
	DWORD dwPrivateKeyLength = 0;	//私钥长度

	//生成公钥和私钥
	GenerateKey(&pPublicKey, &dwPublicKeyLength, &pPrivateKey, &dwPrivateKeyLength);
	//显示公钥
	temp = ByteToCString(pPublicKey, dwPublicKeyLength);
	m_Edit_Data += "公钥：\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";
	//显示私钥
	temp = ByteToCString(pPrivateKey, dwPrivateKeyLength);
	m_Edit_Data += "私钥：\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";

	DWORD dwBufferLength = 4096;	//缓冲区长度
	//公钥加密
	RsaEncrypt(pPublicKey, dwPublicKeyLength, pData, dwDataLength, dwBufferLength);
	//显示加密后的数据
	temp = ByteToCString(pData, dwDataLength);
	m_Edit_Data += "RSA加密后的数据：\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";

	//私钥解密
	RsaDecrypt(pPrivateKey, dwPrivateKeyLength, pData, dwDataLength);
	//显示解密后的数据
	temp = ByteToCString(pData, dwDataLength);
	m_Edit_Data += "RSA解密后的数据：\r\n";
	m_Edit_Data += temp;
	m_Edit_Data += "\r\n";

	UpdateData(FALSE);
}

//获取文件数据
BOOL CEncryptionDlg::GetFileData(char * pszFilePath, BYTE** ppFileData, DWORD * pdwFileDataLength)
{
	//打开文件并获取数据
	HANDLE hFile = CreateFileA(pszFilePath, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("CreateFileA Error！"));
		return FALSE;
	}

	//获取文件大小
	DWORD dwFileDataLength = GetFileSize(hFile, NULL);
	//根据文件大小申请空间
	BYTE* pFileData = new BYTE[dwFileDataLength]{ 0 };
	if (pFileData == NULL)
	{
		MessageBox(_T("new Error！"));
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwTemp = 0;
	//读取文件到申请的空间
	ReadFile(hFile, pFileData, dwFileDataLength, &dwTemp, NULL);
	if ((0 >= dwTemp) || dwTemp != dwFileDataLength)
	{
		delete[] pFileData;
		pFileData = NULL;
		MessageBox(_T("ReadFile Error！"));
		CloseHandle(hFile);
		return FALSE;
	}

	//返回数据
	*ppFileData = pFileData;
	*pdwFileDataLength = dwFileDataLength;

	if (hFile)
	{
		CloseHandle(hFile);
	}
	return TRUE;
}

BOOL CEncryptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//管理员模式下取消过滤拖拽消息
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);       // 0x0049 == WM_COPYGLOBALDATA

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CEncryptionDlg::OnDropFiles(HDROP hDropInfo)
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
CString CEncryptionDlg::ByteToCString(BYTE* str, DWORD dwLength)
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

//计算hash值
BOOL CEncryptionDlg::CalculateHash(BYTE *pData, DWORD dwDataLength, ALG_ID algHashType, BYTE **ppHashData, DWORD *pdwHashDataLength)
{
	BOOL bRet = FALSE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	DWORD dwTemp = 0;
	DWORD dwHashDataLength = 0;
	BYTE* pHashData = NULL;

	// 获得指定CSP的密钥容器的句柄
	bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
	if (FALSE == bRet)
	{
		MessageBox(_T("CryptAcquireContext Error\r\n"));
		return FALSE;
	}

	// 创建一个HASH对象, 指定HASH算法
	bRet = ::CryptCreateHash(hCryptProv, algHashType, NULL, NULL, &hCryptHash);
	if (FALSE == bRet)
	{
		MessageBox(_T("CryptCreateHash Error\r\n"));
		CryptReleaseContext(hCryptProv, 0);
		return FALSE;
	}

	// 计算HASH数据
	bRet = ::CryptHashData(hCryptHash, pData, dwDataLength, 0);
	if (FALSE == bRet)
	{
		MessageBox(_T("CryptHashData Error\r\n"));
		CryptDestroyHash(hCryptHash);
		CryptReleaseContext(hCryptProv, 0);
		return FALSE;
	}

	// 获取HASH结果的大小
	dwTemp = sizeof(dwHashDataLength);
	bRet = ::CryptGetHashParam(hCryptHash, HP_HASHSIZE, (BYTE *)(&dwHashDataLength), &dwTemp, 0);
	if (FALSE == bRet)
	{
		MessageBox(_T("CryptGetHashParam Error\r\n"));
		CryptDestroyHash(hCryptHash);
		CryptReleaseContext(hCryptProv, 0);
		return FALSE;
	}

	// 申请内存
	pHashData = new BYTE[dwHashDataLength]{ 0 };
	if (NULL == pHashData)
	{
		MessageBox(_T("new Error\r\n"));
		CryptDestroyHash(hCryptHash);
		CryptReleaseContext(hCryptProv, 0);
		return FALSE;
	}

	// 获取HASH结果数据
	bRet = ::CryptGetHashParam(hCryptHash, HP_HASHVAL, pHashData, &dwHashDataLength, 0);
	if (FALSE == bRet)
	{
		MessageBox(_T("CryptGetHashParam Error\r\n"));
		delete[] pHashData;
		pHashData = NULL;
		CryptDestroyHash(hCryptHash);
		CryptReleaseContext(hCryptProv, 0);
		return FALSE;
	}

	// 返回数据
	*ppHashData = pHashData;
	*pdwHashDataLength = dwHashDataLength;

	// 释放关闭
	CryptDestroyHash(hCryptHash);
	CryptReleaseContext(hCryptProv, 0);

	return TRUE;
}

// AES加密
BOOL CEncryptionDlg::AesEncrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do {
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptAcquireContext Error\r\n"));
			break;
		}

		// 创建HASH对象
		bRet = ::CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptCreateHash Error\r\n"));
			break;
		}

		// 对密钥进行HASH计算 计算出密钥的MD5值
		bRet = ::CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptHashData Error\r\n"));
			break;
		}

		// 使用HASH来生成密钥
		bRet = ::CryptDeriveKey(hCryptProv, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptDeriveKey Error\r\n"));
			break;
		}
		// 加密数据
		bRet = ::CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptEncrypt Error\r\n"));
			break;
		}
	} while (FALSE);

	// 关闭释放
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptHash)
	{
		CryptDestroyHash(hCryptHash);
	}
	if (hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}

// AES解密
BOOL CEncryptionDlg::AesDecrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength)
{
	// 变量
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptAcquireContext Error\r\n"));
			break;
		}

		// 创建HASH对象
		bRet = ::CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptCreateHash Error\r\n"));
			break;
		}

		// 对密钥进行HASH计算
		bRet = ::CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptCreateHash Error\r\n"));
			break;
		}

		// 使用HASH来生成密钥
		bRet = ::CryptDeriveKey(hCryptProv, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptDeriveKey Error\r\n"));
			break;
		}

		// 解密数据
		bRet = ::CryptDecrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptDecrypt Error\r\n"));
			break;
		}
	} while (FALSE);

	// 关闭释放 
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptHash)
	{
		CryptDestroyHash(hCryptHash);
	}
	if (hCryptProv)
	{

		CryptReleaseContext(hCryptProv, 0);
	}
	return bRet;
}

// 生成公钥和私钥
BOOL CEncryptionDlg::GenerateKey(BYTE **ppPublicKey, DWORD *pdwPublicKeyLength, BYTE **ppPrivateKey, DWORD *pdwPrivateKeyLength)
{
	// 变量
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;
	DWORD dwPublicKeyLength = 0;
	BYTE* pPublicKey = NULL;
	DWORD dwPrivateKeyLength = 0;
	BYTE* pPrivateKey = NULL;

	do
	{
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptAcquireContext Error\r\n"));
			break;
		}

		// 生成公/私密钥对
		bRet = ::CryptGenKey(hCryptProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hCryptKey);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptGenKey Error\r\n"));
			break;
		}

		// 获取公钥密钥的长度和内容
		bRet = ::CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, NULL, &dwPublicKeyLength);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptExportKey Error\r\n"));
			break;
		}
		pPublicKey = new BYTE[dwPublicKeyLength]{0};
		bRet = ::CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, pPublicKey, &dwPublicKeyLength);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptExportKey Error\r\n"));
			break;
		}

		// 获取私钥密钥的长度和内容
		bRet = ::CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, NULL, &dwPrivateKeyLength);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptExportKey Error\r\n"));
			break;
		}
		pPrivateKey = new BYTE[dwPrivateKeyLength]{0};
		bRet = ::CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, pPrivateKey, &dwPrivateKeyLength);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptExportKey Error\r\n"));
			break;
		}

		// 返回数据
		*ppPublicKey = pPublicKey;
		*pdwPublicKeyLength = dwPublicKeyLength;
		*ppPrivateKey = pPrivateKey;
		*pdwPrivateKeyLength = dwPrivateKeyLength;

	} while (FALSE);

	// 释放关闭
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}
	return bRet;
}

// 公钥加密数据
BOOL CEncryptionDlg::RsaEncrypt(BYTE *pPublicKey, DWORD dwPublicKeyLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength)
{
	// 变量
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptAcquireContext Error\r\n"));
			break;
		}

		// 导入公钥
		bRet = ::CryptImportKey(hCryptProv, pPublicKey, dwPublicKeyLength, NULL, 0, &hCryptKey);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptImportKey Error\r\n"));
			break;
		}

		// 加密数据
		bRet = ::CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptEncrypt Error\r\n"));
			break;
		}

	} while (FALSE);

	// 关闭句柄
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}
	return bRet;
}

// 私钥解密数据
BOOL CEncryptionDlg::RsaDecrypt(BYTE *pPrivateKey, DWORD dwProvateKeyLength, BYTE *pData, DWORD &dwDataLength)
{
	// 变量
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptAcquireContext Error\r\n"));
			break;
		}

		// 导入私钥
		bRet = ::CryptImportKey(hCryptProv, pPrivateKey, dwProvateKeyLength, NULL, 0, &hCryptKey);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptImportKey Error\r\n"));
			break;
		}

		// 解密数据
		bRet = ::CryptDecrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength);
		if (FALSE == bRet)
		{
			MessageBox(_T("CryptDecrypt Error\r\n"));
			break;
		}

	} while (FALSE);

	// 关闭句柄
	if (hCryptKey)
	{
		CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}