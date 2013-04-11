#include "stdafx.h"
#include "WndContainer.h"

CWndContainer::CWndContainer()
{
    memset(&m_rcMargin, 0, sizeof(m_rcMargin));
    m_pfnRelayoutMethod = NULL;
}

CWndContainer::~CWndContainer()
{}

void CWndContainer::Init(HWND hWndParent, RECT rcMargin, RelayoutMethod method)
{
    m_rcMargin = rcMargin;
    m_hWndParent = hWndParent;
    m_pfnRelayoutMethod = method;
}

void CWndContainer::OnChanged()
{
    if(m_pfnRelayoutMethod != NULL)
    {
        HDWP hDWP = ::BeginDeferWindowPos(m_HwndVector.size());
        m_pfnRelayoutMethod(hDWP, m_hWndParent, m_rcMargin, m_HwndVector);
        ::EndDeferWindowPos(hDWP);
    }
}

void CWndContainer::Add(HWND hWnd, int nMargin)
{
    stWndContainerItemInfo info = {hWnd, nMargin};
    m_HwndVector.push_back(info);
    OnChanged();
}

void CWndContainer::Insert(HWND hWnd, size_t nIndex, int nMargin)
{
    if(nIndex > m_HwndVector.size())
        nIndex = m_HwndVector.size();
    stWndContainerItemInfo info = {hWnd, nMargin};
    m_HwndVector.insert(m_HwndVector.begin() + nIndex, info);
    OnChanged();
}

void CWndContainer::Remove(HWND hWnd)
{
    HwndVector::iterator ite = m_HwndVector.begin();
    for(; ite != m_HwndVector.end(); ++ ite)
    {
        if(ite->hWnd == hWnd)
        {
            m_HwndVector.erase(ite);
            OnChanged();
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
void HorzWindowMethod(HDWP hDWP, HWND hWndParent, const RECT rcMargin, const HwndVector& vctWindows)
{
    int nLeft = rcMargin.left;
    int nTop = rcMargin.top;
    RECT rcWnd;
    HwndVector::const_iterator ite = vctWindows.begin();
    for(; ite != vctWindows.end(); ++ ite)
    {
        const stWndContainerItemInfo& info = *ite;
        ::GetWindowRect(info.hWnd, &rcWnd);

        ::DeferWindowPos(hDWP,
            info.hWnd,
            NULL, 
            nLeft, nTop,
            rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top,
            SWP_NOZORDER
            );
        nLeft += (rcWnd.right - rcWnd.left) + info.nMargin;
    }
}
