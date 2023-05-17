
// ImageClassifyDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ImageClassify.h"
#include "ImageClassifyDlg.h"
#include "afxdialogex.h"
#include <string>
#include "TaskManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CImageClassifyDlg 对话框



CImageClassifyDlg::CImageClassifyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGECLASSIFY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CImageClassifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageClassifyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SINGLE, &CImageClassifyDlg::OnBnClickedButtonSingle)
	ON_BN_CLICKED(IDC_BUTTON_START_SINGLE, &CImageClassifyDlg::OnBnClickedButtonStartSingle)
	ON_BN_CLICKED(IDC_BUTTON_DIR, &CImageClassifyDlg::OnBnClickedButtonDir)
	ON_BN_CLICKED(IDC_BUTTON_START_DIR, &CImageClassifyDlg::OnBnClickedButtonStartDir)
	ON_EN_CHANGE(IDC_EDIT_NORMALLIGHT_DIR, &CImageClassifyDlg::OnEnChangeEditNormallightDir)
END_MESSAGE_MAP()


// CImageClassifyDlg 消息处理程序

BOOL CImageClassifyDlg::OnInitDialog()
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

	m_oFont.CreatePointFont(150, _T("宋体"));
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImageClassifyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImageClassifyDlg::OnPaint()
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
HCURSOR CImageClassifyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImageClassifyDlg::OnBnClickedButtonSingle()
{
	// TODO: 在此添加控件通知处理程序代码
	CString OpenFilter = _T("图片文件(*.*)|*.*|");

	CFileDialog FileOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, OpenFilter);
	if (IDOK == FileOpen.DoModal())
	{
		CString PathName = FileOpen.GetPathName();
		GetDlgItem(IDC_EDIT_SINGLE)->SetWindowTextW(PathName);
		GetDlgItem(IDC_EDIT_SINGLE)->SetFont(&m_oFont);
	}

}


void CImageClassifyDlg::OnBnClickedButtonStartSingle()
{
	// TODO: 在此添加控件通知处理程序代码

	//文本框读取
	CString  strSourceFile;
	CEdit* pEditSourceFile = (CEdit*)GetDlgItem(IDC_EDIT_SINGLE);
	if (pEditSourceFile)
	{
		pEditSourceFile->GetWindowText(strSourceFile);
	}

	//分类
	CTaskManager* pManager = new CTaskManager;
	IMAGE_TYPE type = pManager->AddTask(strSourceFile.GetBuffer());

	CEdit* pEditResult = (CEdit*)GetDlgItem(IDC_EDIT_RESULT_SINGLE);
	if (type == NORMAL_IMAGE)
	{
		pEditResult->SetWindowText(L"正常");
	}else if (type == AG_IMAGE)
	{
		pEditResult->SetWindowText(L"暗光");
	}
	else if (type == NG_IMAGE)
	{
		pEditResult->SetWindowText(L"逆光");
	}
	else if (type == FG_IMAGE)
	{
		pEditResult->SetWindowText(L"反光");
	}
}


void CImageClassifyDlg::OnBnClickedButtonDir()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("从下面选文件夹目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS; //BIF_BROWSEINCLUDEFILES
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	if (szBuffer)
	{
		std::wstring strFilePath = szBuffer;

		//文本框写入
		CEdit* pEditSourceFileDir = (CEdit*)GetDlgItem(IDC_EDIT_DIR);
		if (pEditSourceFileDir)
		{
			pEditSourceFileDir->SetWindowText(szBuffer);
			pEditSourceFileDir->SetFont(&m_oFont);
		}
	}


}


void CImageClassifyDlg::OnBnClickedButtonStartDir()
{
	// TODO: 在此添加控件通知处理程序代码

	//文本框读取
	CString  strSourceDir;
	CEdit* pEditSourceDir = (CEdit*)GetDlgItem(IDC_EDIT_DIR);
	if (pEditSourceDir)
	{
		pEditSourceDir->GetWindowText(strSourceDir);
	}
	//分类
	CTaskManager* pManager = new CTaskManager;
	pManager->AddTask(strSourceDir.GetBuffer());
}


void CImageClassifyDlg::OnEnChangeEditNormallightDir()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
