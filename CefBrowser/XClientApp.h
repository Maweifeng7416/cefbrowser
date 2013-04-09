#pragma once




class XClientApp : public CefApp,
    public CefBrowserProcessHandler,
    public CefRenderProcessHandler
{
public:
    XClientApp(void);
    ~XClientApp(void);

    IMPLEMENT_REFCOUNTING(ClientApp);
};
