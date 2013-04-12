
// CefBrowser.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "CefBrowser.h"
#include "CefBrowserDlg.h"

#include "XClientApp.h"
#include "XUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCefBrowserApp

BEGIN_MESSAGE_MAP(CCefBrowserApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCefBrowserApp 构造

CCefBrowserApp::CCefBrowserApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCefBrowserApp 对象

CCefBrowserApp theApp;


// CCefBrowserApp 初始化

BOOL CCefBrowserApp::InitInstance()
{
    {
        CefMainArgs main_args(::GetModuleHandle(NULL));
        CefRefPtr<XClientApp> app(new XClientApp);

        // Execute the secondary process, if any.
        int exit_code = CefExecuteProcess(main_args, app.get());
        if (exit_code >= 0)
            return exit_code;

        // Parse command line arguments. The passed in values are ignored on Windows.
        AppInitCommandLine(0, NULL);

        CefSettings settings;

        // Populate the settings based on command line arguments.
        AppGetSettings(settings);

        // Initialize CEF.
        CefInitialize(main_args, settings, app.get());

        // Init plugins, like Flash etc.
        InitWebPlugins();
    }

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	CCefBrowserDlg* pDlg = new CCefBrowserDlg;
    m_pMainWnd = pDlg;
    pDlg->Create(MAKEINTRESOURCE(IDD_CEFBROWSER_DIALOG), NULL);
    m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
    m_pMainWnd->UpdateWindow();

    {
        // Run the CEF message loop. This function will block until the application
        // recieves a WM_QUIT message.
        CefRunMessageLoop();

        CefShutdown();
    }

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
