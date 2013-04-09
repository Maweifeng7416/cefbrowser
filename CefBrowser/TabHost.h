#pragma once

#include <map>

struct stTabInfo
{
    HWND hWndButton;
    HWND hWndBrowser;
    CefRefPtr<CefBrowser>   browser;
};
typedef std::map<HWND, stTabInfo> TabInfoMap;

class CTabHost
{
public:
    CTabHost(void);
    ~CTabHost(void);

    void Init(HWND hWndHost);
    void Destroy();

    void ShowTab(DWORD hTabButton);
    void AddTab(HWND hTabButton, HWND hWndBrowser);
    void DeleteTab(HWND hWndTabButton);

protected:
    HWND    m_hWndHost;

    HWND    m_hVisibleTabButton;
    TabInfoMap  m_TabInfoMap;
};
