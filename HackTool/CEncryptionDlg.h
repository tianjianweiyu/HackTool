#pragma once
#include <wincrypt.h>


// CEncryptionDlg 对话框

class CEncryptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEncryptionDlg)

public:
	CEncryptionDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEncryptionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENCRYPTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//************************************
	// 函数名：CEncryptionDlg::CalculateHash
	// 返回类型：BOOL
	// 功能:	计算hash值
	// 参数1：BYTE *pData 需要计算哈希值的数据
	// 参数2：DWORD dwDataLength 需要计算哈希值的数据长度
	// 参数3：ALG_ID algHashType 需要计算哈希值的数据的类型
	// 参数4：BYTE **ppHashData 计算出来的哈希数据
	// 参数5：DWORD *pdwHashDataLength 计算出来的哈希数据长度
	//************************************
	BOOL CalculateHash(BYTE *pData, DWORD dwDataLength, ALG_ID algHashType, BYTE **ppHashData, DWORD *pdwHashDataLength);

	//************************************
	// 函数名：CEncryptionDlg::GetFileData
	// 返回类型：BOOL
	// 功能:	获取文件数据
	// 参数1：char * pszFilePath 文件路径
	// 参数2：BYTE** ppFileData 文件数据首地址
	// 参数3：WORD * pdwFileDataLength 文件数据长度
	//************************************
	BOOL GetFileData(char * pszFilePath, BYTE** ppFileData, DWORD * pdwFileDataLength);

	//************************************
	// 函数名：CEncryptionDlg::ByteToCString
	// 返回类型：CString
	// 功能:	把BYTE* 转换为十六进制显示的CString
	// 参数1：BYTE* str	需要转换的BYTE*
	// 参数2：DWORD dwLength	需要转换的BYTE*的长度
	//************************************
	CString ByteToCString(BYTE* str, DWORD dwLength);

	//************************************
	// 函数名：CEncryptionDlg::AesEncrypt
	// 返回类型：BOOL
	// 功能:	AES加密
	// 参数1：BYTE *pPassword 密钥
	// 参数2：DWORD dwPasswordLength 密钥长度
	// 参数3：BYTE *pData 需要AES加密的数据
	// 参数4：DWORD &dwDataLength 需要AES加密的数据长度
	// 参数5：DWORD dwBufferLength  缓冲区长度
	//************************************
	BOOL AesEncrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength);

	//************************************
	// 函数名：CEncryptionDlg::AesDecrypt
	// 返回类型：BOOL
	// 功能:	AES解密
	// 参数1：BYTE *pPassword 密钥
	// 参数2：DWORD dwPasswordLength 密钥长度
	// 参数3：BYTE *pData 需要AES解密的数据
	// 参数4：DWORD &dwDataLength 需要AES解密的数据长度
	//************************************
	BOOL AesDecrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength);

	//************************************
	// 函数名：CEncryptionDlg::GenerateKey
	// 返回类型：BOOL
	// 功能:	生成公钥和私钥
	// 参数1：BYTE **ppPublicKey 公钥
	// 参数2：DWORD *pdwPublicKeyLength 公钥长度
	// 参数3：BYTE **ppPrivateKey 私钥
	// 参数4：DWORD *pdwPrivateKeyLength 私钥长度
	//************************************
	BOOL GenerateKey(BYTE **ppPublicKey, DWORD *pdwPublicKeyLength, BYTE **ppPrivateKey, DWORD *pdwPrivateKeyLength);

	//************************************
	// 函数名：CEncryptionDlg::RsaEncrypt
	// 返回类型：BOOL
	// 功能:	RAS加密
	// 参数1：BYTE *pPublicKey	公钥
	// 参数2：DWORD dwPublicKeyLength	公钥长度
	// 参数3：BYTE *pData	需要加密的数据
	// 参数4：DWORD &dwDataLength	需要加密的数据长度
	// 参数5：DWORD dwBufferLength	缓冲区长度
	//************************************
	BOOL RsaEncrypt(BYTE *pPublicKey, DWORD dwPublicKeyLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength);

	//************************************
	// 函数名：CEncryptionDlg::RsaDecrypt
	// 返回类型：BOOL
	// 功能:	 RAS解密
	// 参数1：BYTE **ppPrivateKey 私钥
	// 参数2：DWORD *pdwPrivateKeyLength 私钥长度
	// 参数3：BYTE *pData	需要解密的数据
	// 参数4：DWORD &dwDataLength	需要解密的数据长度
	//************************************
	BOOL RsaDecrypt(BYTE *pPrivateKey, DWORD dwProvateKeyLength, BYTE *pData, DWORD &dwDataLength);

public:
	afx_msg void OnBnClickedButton1();
	CString m_Edit_Path;
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_Edit_Data;
	afx_msg void OnBnClickedButton2();
	CString m_Edit_Aes;
	afx_msg void OnBnClickedButton8();
};
