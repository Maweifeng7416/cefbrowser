#pragma once

#include <vector>

struct stTabInfo
{
    HWND hWndButton;
    HWND hWndBrowser;
    int  BrowserId;
    CefRefPtr<CefBrowser>   browser;
};
typedef std::vector<stTabInfo> TabInfoVector;

class CTabHost
{
public:
    CTabHost();
    ~CTabHost(void);

    void Init(HWND hWndHost, HWND hEditUrlWnd);
    void Destroy();

    void ShowTab(HWND hTabButton);

    void CreateTab(LPCTSTR szUrl);
    void AddTab(CefRefPtr<CefBrowser> browser, CefWindowInfo* info);
    void DeleteTab(HWND hWndTabButton);

    void OpenUrl(LPCTSTR szUrl);

    void OnAddressChange(CefRefPtr<CefBrowser> browser, const CefString& url);
    void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title);

protected:
    void GetBrowserRect(RECT& rcBrowser);
    void AddTabImpl(CefRefPtr<CefBrowser> browser);
    static void AddTabTask(CTabHost* pTabHost, CefRefPtr<CefBrowser> browser);

    stTabInfo* GetTabInfoByButton(HWND hWndButton);
    stTabInfo* GetTabInfoByBrowser(const CefRefPtr<CefBrowser> browser);

protected:
    HWND    m_hWndHost;
    HWND    m_hEditUrlWnd;

    HWND    m_hVisibleTabButton;
    HWND    m_hVisibleBrowser;

    TabInfoVector  m_vctTabInfo;

    HWND        m_hBtnContainer;
    CWndContainer m_BtnLayout;

    HWND        m_hBrowContainer;
    CWndLayout  m_BrowLayout;
};
