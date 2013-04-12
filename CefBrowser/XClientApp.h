#pragma once




class XClientApp : public CefApp,
    public CefRenderProcessHandler
{
public:
    XClientApp(void);
    ~XClientApp(void);

    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() {
        return this;
    }

    // CefRenderProcessHandler
    virtual void OnWebKitInitialized();
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
        CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message);

    IMPLEMENT_REFCOUNTING(XClientApp);
};
