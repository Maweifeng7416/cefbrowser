#pragma once

#include <vector>
#include <algorithm>


struct stWndContainerItemInfo
{
    HWND hWnd;
    int  nMargin;
};
typedef std::vector<stWndContainerItemInfo> HwndVector;
typedef void (*RelayoutMethod)(HDWP hDWP, HWND hWndParent, const RECT rcMargin, const HwndVector& vctWindows);

void HorzWindowMethod(HDWP hDWP, HWND hWndParent, const RECT rcMargin, const HwndVector& vctWindows);

class CWndContainer
{
    CWndContainer(const CWndContainer&);
    CWndContainer& operator = (const CWndContainer&);

public:
    CWndContainer();
    ~CWndContainer();

    void Init(HWND hWndParent, RECT rcMargin, RelayoutMethod method);
    void OnChanged();

    void Add(HWND hWnd, int nMargin);
    void Insert(HWND hWnd, size_t nIndex, int nMargin);
    void Remove(HWND hWnd);

protected:
    RECT            m_rcMargin;
    HWND            m_hWndParent;
    HwndVector      m_HwndVector;
    RelayoutMethod  m_pfnRelayoutMethod;
};
