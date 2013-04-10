#include "StdAfx.h"
#include "TabHost.h"


#include "resource.h"
#include "XGlobal.h"

CTabHost::CTabHost()
{
    m_hEditUrlWnd = m_hWndHost = m_hVisibleBrowser = m_hVisibleTabButton = NULL;
}

CTabHost::~CTabHost(void)
{
}

void CTabHost::Init(HWND hWndHost, HWND hEditUrlWnd)
{
    m_hWndHost = hWndHost;
    m_hEditUrlWnd = hEditUrlWnd;

    // m_hBtnContainer = ::GetDlgItem(m_hWndHost, IDC_FRAME_BUTTONS);
    m_hBtnContainer = m_hWndHost;
    RECT rcButtons = {0, 30, 0, 0};
    m_BtnLayout.Init(m_hWndHost, rcButtons, &HorzWindowMethod);


    m_hBrowContainer = ::GetDlgItem(m_hWndHost, IDC_FRAME_BROWSER);
    m_BrowLayout.Init(m_hBrowContainer);
}

void CTabHost::Destroy()
{
    m_hEditUrlWnd = m_hWndHost = m_hVisibleBrowser = m_hVisibleTabButton = NULL;

    TabInfoVector::iterator iteBrowser = m_vctTabInfo.begin();
    for(; iteBrowser != m_vctTabInfo.end(); ++ iteBrowser)
    {
        stTabInfo& info = *iteBrowser;
        if(info.browser.get())
        {
            info.browser->GetHost()->ParentWindowWillClose();
        }
    }
    m_vctTabInfo.clear();
}

void CTabHost::ShowTab(HWND hTabButton)
{
    if(hTabButton == m_hVisibleTabButton)
        return;

    DWORD dwFlag = SW_HIDE;
    for(int i=0; i<2; ++ i)
    {
        if(m_hVisibleTabButton != NULL)
        {
            TabInfoVector::iterator itePrev = m_vctTabInfo.begin();
            for(; itePrev != m_vctTabInfo.end(); ++ itePrev)
            {
                stTabInfo& info = *itePrev;
                if(info.hWndButton == m_hVisibleTabButton)
                    break;
            }
            if(itePrev == m_vctTabInfo.end())
            {
                continue;
            }

            stTabInfo& info = *itePrev;
            // ::SetWindowPos(info.hWndBrowser, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | dwFlag | SWP_ASYNCWINDOWPOS);
            ::ShowWindow(info.hWndBrowser, dwFlag);
            if(dwFlag == SW_SHOW)
            {
                ::SendMessage(m_hVisibleTabButton, BM_SETCHECK, BST_CHECKED, 0);
                m_hVisibleBrowser = info.hWndBrowser;
            }
            else
            {
                ::SendMessage(m_hVisibleTabButton, BM_SETCHECK, BST_UNCHECKED, 0);
            }
        }
        dwFlag = SW_SHOW;
        m_hVisibleTabButton = hTabButton;
    }
}

void CTabHost::CreateTab(LPCTSTR szUrl)
{
    CefWindowInfo info;
    CefBrowserSettings settings;

    // Initialize window info to the defaults for a child window.
    RECT rcBrowser = {0};
    GetBrowserRect(rcBrowser);

    info.SetAsChild(m_hBrowContainer, rcBrowser);

    // Creat the new child browser window
    CefRefPtr<CefBrowser> browser;
    CefBrowserHost::CreateBrowser(info, XGlobal::inst().ClientHandler.get(), szUrl, settings);

    ::SetWindowText(m_hEditUrlWnd, szUrl);
}

void CTabHost::AddTab(CefRefPtr<CefBrowser> browser, CefWindowInfo* info)
{
    RECT rcBrowser = {0};
    GetBrowserRect(rcBrowser);

    if(info != NULL)
    {
        info->style &= (~WS_VISIBLE);
        info->SetAsChild(m_hBrowContainer, rcBrowser);
    }
    else
    {
        CefPostTask(TID_UI, NewCefRunnableFunction(&CTabHost::AddTabTask, this, browser));
    }
}

void CTabHost::AddTabTask(CTabHost* pTabHost, CefRefPtr<CefBrowser> browser)
{
    pTabHost->AddTabImpl(browser);
}

void CTabHost::AddTabImpl(CefRefPtr<CefBrowser> browser)
{
    stTabInfo tab;

    HWND hWndButton = ::CreateWindow(WC_BUTTON,
        _T("ÕýÔÚ¼ÓÔØ..."),
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | BS_LEFT | BS_FLAT | WS_BORDER,
        0, 0,
        100, 22,
        m_hBtnContainer,
        (HMENU)IDC_TAB_BUTTON,
        ::GetModuleHandle(NULL),
        0);
    ::SendMessage(hWndButton, WM_SETFONT, (WPARAM)::SendMessage(m_hWndHost, WM_GETFONT, 0, 0), TRUE);
    m_BtnLayout.Add(hWndButton, 2);

    // Initialize window info to the defaults for a child window.
    RECT rcBrowser = {0};
    GetBrowserRect(rcBrowser);

    // Creat the new child browser window
    tab.hWndButton = hWndButton;
    tab.browser = browser;
    tab.hWndBrowser = tab.browser->GetHost()->GetWindowHandle();

    m_BrowLayout.AddControlByHwnd(tab.hWndBrowser, Layout_HFill | Layout_VFill);

    m_vctTabInfo.push_back(tab);
    XOutputLog1(_T("AddTab"), _T("Button: 0x%08x, Browser: 0x%08x"), tab.hWndButton, tab.hWndBrowser);

    ShowTab(hWndButton);
}

void CTabHost::DeleteTab(HWND hWndTabButton)
{
    ;
}

void CTabHost::OpenUrl(LPCTSTR szUrl)
{
    stTabInfo* info = GetTabInfoByButton(m_hVisibleTabButton);
    if(info == NULL)
    {
        CreateTab(szUrl);
        return;
    }
    else
    {
        info->browser->GetMainFrame()->LoadURL(szUrl);
    }
}

void CTabHost::GetBrowserRect(RECT& rcBrowser)
{
    ::GetClientRect(m_hBrowContainer, &rcBrowser);
    ::OffsetRect(&rcBrowser, -rcBrowser.left, -rcBrowser.top);
}

void CTabHost::OnAddressChange(CefRefPtr<CefBrowser> browser, const CefString& url)
{
    if(browser->GetHost()->GetWindowHandle() != m_hVisibleBrowser)
        return;

    ::SetWindowText(m_hEditUrlWnd, url.c_str());
}

void CTabHost::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
    stTabInfo* info = GetTabInfoByBrowser(browser);
    if(info != NULL)
        SetWindowText(info->hWndButton, title.c_str());

    if(browser->GetHost()->GetWindowHandle() != m_hVisibleBrowser)
        return;

    SetWindowText(m_hWndHost, title.c_str());
}

stTabInfo* CTabHost::GetTabInfoByButton(HWND hWndButton)
{
    TabInfoVector::iterator ite = m_vctTabInfo.begin();
    for(; ite != m_vctTabInfo.end(); ++ ite)
    {
        stTabInfo& info = *ite;
        if(info.hWndButton == hWndButton)
            return &info;
    }
    return NULL;
}

stTabInfo* CTabHost::GetTabInfoByBrowser(const CefRefPtr<CefBrowser> browser)
{
    TabInfoVector::iterator ite = m_vctTabInfo.begin();
    for(; ite != m_vctTabInfo.end(); ++ ite)
    {
        stTabInfo& info = *ite;
        if(info.browser->IsSame(browser))
            return &info;
    }
    return NULL;
}

