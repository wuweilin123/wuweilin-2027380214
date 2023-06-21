
// MFCApplication1Dlg.cpp: 实现文件
//吴伟林

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"

#include <string>

#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment (lib, "SocketJSONDLL.lib")

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框吴伟林

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1Dlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL isOpen = TRUE;    //是否打开(否则为保存)
	CString defaultDir = L"";    //默认打开的文件路径
	CString fileName = L"";            //默认打开的文件名
	CString filter = L"图像文件 (*.jpg)|*.jpg||";    //文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK)
	{
		CString filePath = openFileDlg.GetPathName();
		CStatic* pStatic = static_cast<CStatic*>(GetDlgItem(IDC_STATIC));  // 获取 IDC_STATIC 控件的指针
		// 加载图像并显示在 IDC_STATIC 控件上
		CImage image;
		image.Load(filePath);
		pStatic->SetBitmap(image);
		HBITMAP hBitmap = (HBITMAP)image;
		pStatic->SetBitmap(hBitmap);
		CWnd::SetDlgItemTextW(IDC_EDIT1, filePath);
		OnBnClickedButton2();
	}
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	// 获取编辑框中的文本
	CString text;
	CWnd* pWnd = GetDlgItem(IDC_EDIT1);
	if (pWnd)
	{
		pWnd->GetWindowText(text);
	}

	// 将CString类型的字符串转换为std::string类型的字符串
	std::string str(CW2A(text.GetString(), CP_UTF8));

	// 加载动态链接库
	HINSTANCE hDll = LoadLibrary(_T("SocketJSONDLL.dll"));
	if (hDll == NULL)
	{

		return;
	}

	// 获取SendData函数指针
	typedef std::string(*SendDataFunc)(const std::string&);
	SendDataFunc sendDataFunc = (SendDataFunc)GetProcAddress(hDll, "SendData");
	if (sendDataFunc == NULL)
	{
		// 获取函数指针失败
		FreeLibrary(hDll);
		return;
	}

	// 吴调伟用林SendData函数

	// 调用SendData函数
	std::string result = sendDataFunc(str);

	// 将分类结果显示在IDC_EDIT2中
	CString resultText;

	if (result == "FG")
	{
		resultText = "反光";
	}
	else if (result == "ZC")
	{
		resultText = "正常";
	}
	else if (result == "GA")
	{
		resultText = "过暗";
	}
	else if (result == "NG")
	{
		resultText = "逆光";
	}
	else//吴伟林
	{
		resultText = "未知结果";
	}

	CWnd* pResultWnd = GetDlgItem(IDC_EDIT3);
	if (pResultWnd)
	{
		pResultWnd->SetWindowTextW(resultText);
	}
	// 卸载动态链接库 2027380214吴伟林
	FreeLibrary(hDll);
}


void CMFCApplication1Dlg::OnBnClickedButton5()
{
	// 打开文件夹选择对话框
	CString defaultDir = L""; // 默认打开的文件夹路径
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lpszTitle = L"请选择图像文件夹";
	bi.hwndOwner = NULL;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		// 获取文件夹路径
		TCHAR szPath[MAX_PATH];
		SHGetPathFromIDList(pidl, szPath);
		CString folderPath = szPath;

		// 找到文件夹中的第一张图像文件并显示
		CFileFind fileFind;
		BOOL bFind = fileFind.FindFile(folderPath + L"\\*.jpg");
		if (bFind)
		{
			fileFind.FindNextFile();
			CString filePath = fileFind.GetFilePath();
			CStatic* pStatic = static_cast<CStatic*>(GetDlgItem(IDC_STATIC));
			CImage image;
			image.Load(filePath);
			pStatic->SetBitmap(image);
			HBITMAP hBitmap = (HBITMAP)image;
			pStatic->SetBitmap(hBitmap);
			CWnd::SetDlgItemTextW(IDC_EDIT1, filePath);
			OnBnClickedButton2();
		}
	}
}

void CMFCApplication1Dlg::OnBnClickedButton3()
{
	// 获取文本框中的文件路径
	CString filePath;
	CWnd* pFilePathWnd = GetDlgItem(IDC_EDIT1);
	if (pFilePathWnd)
	{
		pFilePathWnd->GetWindowTextW(filePath);
	}

	// 获取上一个图像文件的路径
	CString folderPath = filePath.Left(filePath.ReverseFind('\\'));
	CFileFind fileFind;
	BOOL bFind = fileFind.FindFile(folderPath + L"\\*.jpg");
	CString prevFilePath;
	if (bFind)
	{
		while (bFind)
		{
			bFind = fileFind.FindNextFile();
			CString tempFilePath = fileFind.GetFilePath();
			if (tempFilePath == filePath)
			{
				break;
			}
			prevFilePath = tempFilePath;
		}
	}

	// 如果没有找到上一个图像文件，则从最后一个文件开始遍历
	if (prevFilePath.IsEmpty())
	{
		bFind = fileFind.FindFile(folderPath + L"\\*.jpg");
		while (bFind)
		{
			bFind = fileFind.FindNextFile();
			CString tempFilePath = fileFind.GetFilePath();
			if (tempFilePath == filePath)
			{
				break;
			}
			prevFilePath = tempFilePath;
		}
	}

	// 如果找到了上一个图像文件，则显示
	if (!prevFilePath.IsEmpty())
	{
		CStatic* pStatic = static_cast<CStatic*>(GetDlgItem(IDC_STATIC));
		CImage image;
		image.Load(prevFilePath);
		pStatic->SetBitmap(image);
		HBITMAP hBitmap = (HBITMAP)image;
		pStatic->SetBitmap(hBitmap);
		CWnd::SetDlgItemTextW(IDC_EDIT1, prevFilePath);
		OnBnClickedButton2();
	}
}

void CMFCApplication1Dlg::OnBnClickedButton4()
{
	// 获取文本框中的文件路径
	CString filePath;
	CWnd* pFilePathWnd = GetDlgItem(IDC_EDIT1);
	if (pFilePathWnd)
	{
		pFilePathWnd->GetWindowTextW(filePath);
	}

	// 获取下一个图像文件的路径
	CString folderPath = filePath.Left(filePath.ReverseFind('\\'));
	CFileFind fileFind;
	BOOL bFind = fileFind.FindFile(folderPath + L"\\*.jpg");
	CString nextFilePath;
	if (bFind)
	{
		BOOL bFound = false;
		while (bFind)
		{
			bFind = fileFind.FindNextFile();
			CString tempFilePath = fileFind.GetFilePath();
			if (bFound)
			{
				nextFilePath = tempFilePath;
				break;
			}
			if (tempFilePath == filePath)
			{
				bFound = true;
			}
		}
	}

	// 如果没有找到下一个图像文件，则从第一个文件开始遍历
	if (nextFilePath.IsEmpty())
	{
		bFind = fileFind.FindFile(folderPath + L"\\*.jpg");
		BOOL bFound = false;
		while (bFind)
		{
			bFind = fileFind.FindNextFile();
			CString tempFilePath = fileFind.GetFilePath();
			if (bFound)
			{
				nextFilePath = tempFilePath;
				break;
			}
			if (tempFilePath == filePath)
			{
				bFound = true;
			}
		}
	}

	// 如果找到了下一个图像文件，则显示
	if (!nextFilePath.IsEmpty())
	{
		CStatic* pStatic = static_cast<CStatic*>(GetDlgItem(IDC_STATIC));
		CImage image;
		image.Load(nextFilePath);
		pStatic->SetBitmap(image);
		HBITMAP hBitmap = (HBITMAP)image;
		pStatic->SetBitmap(hBitmap);
		CWnd::SetDlgItemTextW(IDC_EDIT1, nextFilePath);
		OnBnClickedButton2();
	}
}