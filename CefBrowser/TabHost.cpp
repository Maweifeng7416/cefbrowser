#include "StdAfx.h"
#include "TabHost.h"

CTabHost::CTabHost(void)
{
    m_hWndHost = m_hVisibleTabButton = NULL;
}

CTabHost::~CTabHost(void)
{
}

void CTabHost::Init(HWND hWndHost)
{
    m_hWndHost = hWndHost;
}

void CTabHost::Destroy()
{
    m_hWndHost = m_hVisibleTabButton = NULL;

    TabInfoMap::iterator iteBrowser = m_TabInfoMap.begin();
    for(; iteBrowser != m_TabInfoMap.end(); ++ iteBrowser)
    {
        HWND hWndButton = iteBrowser->first;
        stTabInfo& info = iteBrowser->second;
        if(info.browser.get())
        {
            info.browser->GetHost()->ParentWindowWillClose();
        }
    }
    m_TabInfoMap.clear();
}

void CTabHost::ShowTab(DWORD hTabButton)
{
    ;
}

void CTabHost::AddTab(HWND hTabButton, HWND hWndBrowser)
{
    ;
}

void CTabHost::DeleteTab(HWND hWndTabButton)
{
    ;
}

