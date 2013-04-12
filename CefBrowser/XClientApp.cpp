#include "StdAfx.h"
#include "XClientApp.h"
#include "XUtil.h"
#include "XAppExtentionHandler.h"

XClientApp::XClientApp(void)
{
}

XClientApp::~XClientApp(void)
{
}

//////////////////////////////////////////////////////////////////////////
// CefRenderProcessHandler
void XClientApp::OnWebKitInitialized()
{
    CefString strJsCode;
    bool bResult = LoadStringResource(RT_HTML, L"app.js", strJsCode);

    if(bResult)
    {
        bResult = CefRegisterExtension(L"v8/app", strJsCode, new XAppExtentionHandler);
    }
}

bool XClientApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                      CefProcessId source_process,
                                      CefRefPtr<CefProcessMessage> message)
{
    return false;
}