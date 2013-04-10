#pragma once




class XClientApp : public CefApp,
    public CefBrowserProcessHandler,
    public CefRenderProcessHandler
{
public:
    XClientApp(void);
    ~XClientApp(void);

    virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser);

    IMPLEMENT_REFCOUNTING(ClientApp);
};
