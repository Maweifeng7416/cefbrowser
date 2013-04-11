
// CefBrowserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CefBrowser.h"
#include "CefBrowserDlg.h"

#include "XGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCefBrowserDlg 对话框


//////////////////////////////////////////////////////////////////////////

WNDPROC g_OldEditProc = NULL;
LRESULT WINAPI EditWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_CHAR && wParam == VK_RETURN)
    {
        ::PostMessage(::GetParent(hWnd), WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), (LPARAM)hWnd);
        return 0;
    }
    return ::CallWindowProc(g_OldEditProc, hWnd, message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////


CCefBrowserDlg::CCefBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCefBrowserDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCefBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCefBrowserDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_TAB_BUTTON, &CCefBrowserDlg::OnBtnTabClicked)
    ON_BN_DOUBLECLICKED(IDC_TAB_BUTTON, &CCefBrowserDlg::OnBtnTabDoubleClicked)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CCefBrowserDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BTN_BACK, &CCefBrowserDlg::OnBnClickedBtnBack)
    ON_BN_CLICKED(IDC_BTN_REFRESH, &CCefBrowserDlg::OnBnClickedBtnRefresh)
    ON_BN_CLICKED(IDC_BTN_FORWARD, &CCefBrowserDlg::OnBnClickedBtnForward)
END_MESSAGE_MAP()


// CCefBrowserDlg 消息处理程序

BOOL CCefBrowserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码

    // 
    HWND hEditUrlWnd = GetDlgItem(IDC_EDIT_URL)->GetSafeHwnd();
    g_OldEditProc = (WNDPROC)::SetWindowLongPtr(hEditUrlWnd, GWLP_WNDPROC, (LONG_PTR)&EditWindowProc);

    // 
    XGlobal::inst().WndLayout.Init(m_hWnd);
    XGlobal::inst().WndLayout.AddControlById(IDC_EDIT_URL, Layout_HFill | Layout_Top);
    XGlobal::inst().WndLayout.AddControlById(IDOK, Layout_Top | Layout_Right);
    XGlobal::inst().WndLayout.AddControlById(IDC_FRAME_BROWSER, Layout_HFill | Layout_VFill);

    XGlobal::inst().TabHost.Init(m_hWnd);
    XGlobal::inst().TabHost.OpenUrl(_T("http://www.youku.com"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCefBrowserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCefBrowserDlg::OnPaint()
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
		CPaintDC dc(this);

        RECT rcClient;
        ::GetClientRect(m_hWnd, &rcClient);
        ::FillRect(dc.GetSafeHdc(), &rcClient, (HBRUSH)::GetStockObject(WHITE_BRUSH));
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCefBrowserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCefBrowserDlg::OnClose()
{
    XGlobal::inst().TabHost.Destroy();
    ::PostQuitMessage(0);
    CDialog::OnClose();
}

BOOL CCefBrowserDlg::OnEraseBkgnd(CDC* pDC)
{
    return 1;
}

HBRUSH CCefBrowserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if(nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
    else
    {
        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}

void CCefBrowserDlg::OnBtnTabClicked()
{
    XGlobal::inst().TabHost.ShowTab(::GetFocus());
}

void CCefBrowserDlg::OnBtnTabDoubleClicked()
{
    XGlobal::inst().TabHost.CloseTab(::GetFocus());
}

void CCefBrowserDlg::OnBnClickedOk()
{
    CString strUrl;
    GetDlgItemText(IDC_EDIT_URL, strUrl);
    XGlobal::inst().TabHost.OpenUrl(strUrl);
}

void CCefBrowserDlg::OnBnClickedBtnBack()
{
    XGlobal::inst().TabHost.GoBack();
}

void CCefBrowserDlg::OnBnClickedBtnRefresh()
{
    XGlobal::inst().TabHost.Refresh();
}

void CCefBrowserDlg::OnBnClickedBtnForward()
{
    XGlobal::inst().TabHost.GoForward();
}
