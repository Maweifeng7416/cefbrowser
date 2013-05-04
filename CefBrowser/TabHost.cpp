#include "StdAfx.h"
#include "TabHost.h"


#include "resource.h"
#include "XGlobal.h"

CTabHost::CTabHost()
{
    m_hBtnBack = m_hBtnForward = m_hBtnRefresh =
    m_hEditUrlWnd = m_hWndHost =
    m_hVisibleBrowser = m_hVisibleTabButton = NULL;
}

CTabHost::~CTabHost(void)
{
}

void CTabHost::Init(HWND hWndHost)
{
    m_hWndHost = hWndHost;

    m_hEditUrlWnd = ::GetDlgItem(hWndHost, IDC_EDIT_URL);
    m_hBtnBack = ::GetDlgItem(hWndHost, IDC_BTN_BACK);
    m_hBtnForward = ::GetDlgItem(hWndHost, IDC_BTN_FORWARD);
    m_hBtnRefresh = ::GetDlgItem(hWndHost, IDC_BTN_REFRESH);

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

                // 同步按钮状态
                ::EnableWindow(m_hBtnBack, info.browser->CanGoBack());
                ::EnableWindow(m_hBtnForward, info.browser->CanGoForward());
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

void CTabHost::RemoveTab(CefRefPtr<CefBrowser> browser)
{
    TabInfoVector::iterator ite = GetTabInfoByBrowserEx(browser);
    if(ite == m_vctTabInfo.end())
        return;

    stTabInfo& info = *ite;
    m_BtnLayout.Remove(info.hWndButton);
    m_BrowLayout.RemoveControlByHwnd(info.hWndBrowser);
    ::DestroyWindow(info.hWndButton);
    m_vctTabInfo.erase(ite);

    if(m_hVisibleBrowser == browser->GetHost()->GetWindowHandle())
    {
        m_hVisibleBrowser = m_hVisibleTabButton = NULL;
    }

    if(m_vctTabInfo.size() > 0)
        ShowTab(m_vctTabInfo[0].hWndButton);
}

// 用户要求关闭Tab时，调用本方法
void CTabHost::CloseTab(HWND hWndButton)
{
    TabInfoVector::iterator ite = GetTabInfoByButtonEx(hWndButton);
    if(ite == m_vctTabInfo.end())
        return;

    stTabInfo& info = *ite;
    m_BtnLayout.Remove(info.hWndButton);
    m_BrowLayout.RemoveControlByHwnd(info.hWndBrowser);
    ::DestroyWindow(info.hWndButton);

    if(m_hVisibleBrowser == info.browser->GetHost()->GetWindowHandle())
    {
        m_hVisibleBrowser = m_hVisibleTabButton = NULL;
    }
    info.browser->GetHost()->CloseBrowser();
    m_vctTabInfo.erase(ite);

    if(m_vctTabInfo.size() > 0)
        ShowTab(m_vctTabInfo[0].hWndButton);
}

void CTabHost::AddTabTask(CTabHost* pTabHost, CefRefPtr<CefBrowser> browser)
{
    pTabHost->AddTabImpl(browser);
}

void CTabHost::AddTabImpl(CefRefPtr<CefBrowser> browser)
{
    stTabInfo tab;

    HWND hWndButton = ::CreateWindow(WC_BUTTON,
        _T("正在加载..."),
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | BS_LEFT | BS_FLAT | BS_NOTIFY,
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

    ShowTab(hWndButton);
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

//////////////////////////////////////////////////////////////////////////
// 事件响应
void CTabHost::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
    if(browser->GetHost()->GetWindowHandle() != m_hVisibleBrowser)
        return;

    ::EnableWindow(m_hBtnBack, canGoBack);
    ::EnableWindow(m_hBtnForward, canGoForward);
}

void CTabHost::OnAddressChange(CefRefPtr<CefBrowser> browser, const CefString& url)
{
    // 如果正在编辑URL，则不修改
    if(::GetFocus() == m_hEditUrlWnd)
        return;

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

//////////////////////////////////////////////////////////////////////////
// 浏览器动作
void CTabHost::GoBack()
{
    stTabInfo* info = GetTabInfoByButton(m_hVisibleTabButton);
    if(info == NULL)
        return;

    info->browser->GoBack();
}

void CTabHost::GoForward()
{
    stTabInfo* info = GetTabInfoByButton(m_hVisibleTabButton);
    if(info == NULL)
        return;

    info->browser->GoForward();
}

void CTabHost::Reload(BOOL bIgnoreCache)
{
    stTabInfo* info = GetTabInfoByButton(m_hVisibleTabButton);
    if(info == NULL)
        return;

    if(bIgnoreCache)
        info->browser->ReloadIgnoreCache();
    else
        info->browser->Reload();
}

stTabInfo* CTabHost::GetTabInfoByButton(HWND hWndButton)
{
    TabInfoVector::iterator ite = GetTabInfoByButtonEx(hWndButton);
    if(ite == m_vctTabInfo.end())
        return NULL;
    else
        return (&(*ite));
}

stTabInfo* CTabHost::GetTabInfoByBrowser(const CefRefPtr<CefBrowser> browser)
{
    TabInfoVector::iterator ite = GetTabInfoByBrowserEx(browser);
    if(ite == m_vctTabInfo.end())
        return NULL;
    else
        return (&(*ite));
}

TabInfoVector::iterator CTabHost::GetTabInfoByButtonEx(HWND hWndButton)
{
    TabInfoVector::iterator ite = m_vctTabInfo.begin();
    for(; ite != m_vctTabInfo.end(); ++ ite)
    {
        stTabInfo& info = *ite;
        if(info.hWndButton == hWndButton)
            break;
    }
    return ite;
}

TabInfoVector::iterator CTabHost::GetTabInfoByBrowserEx(const CefRefPtr<CefBrowser> browser)
{
    TabInfoVector::iterator ite = m_vctTabInfo.begin();
    for(; ite != m_vctTabInfo.end(); ++ ite)
    {
        stTabInfo& info = *ite;
        if(info.browser->IsSame(browser))
            break;
    }
    return ite;
}
