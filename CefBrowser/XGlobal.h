#pragma once


#include "TabHost.h"
#include "XClientHandler.h"



class XGlobal
{
    XGlobal(void);
    XGlobal(const XGlobal&);
    XGlobal& operator = (const XGlobal&);
public:
    ~XGlobal(void);

    static XGlobal& inst();

public:
    CTabHost    TabHost;
    CefRefPtr<XClientHandler> ClientHandler;
    CWndLayout      WndLayout;
};
