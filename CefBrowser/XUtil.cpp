#include "StdAfx.h"
#include "XUtil.h"


#include "client_switches.h"
#include "XGlobal.h"


void AppInitCommandLine(int argc, const char* const* argv)
{
    XGlobal::inst().CommandLine = CefCommandLine::CreateCommandLine();
#if defined(OS_WIN)
    XGlobal::inst().CommandLine->InitFromString(::GetCommandLineW());
#else
    XGlobal::inst().CommandLine->InitFromArgv(argc, argv);
#endif
}




// Returns the application settings based on command line arguments.
void AppGetSettings(CefSettings& settings)
{
    ASSERT(XGlobal::inst().CommandLine.get());
    if (!XGlobal::inst().CommandLine.get())
        return;

    std::wstring strAppPath = AppGetAppPath();

    std::wstring strCacheDir = strAppPath + L"caches";
    std::wstring strLogFile = strCacheDir + L"\\CefBrowser.log";

    settings.command_line_args_disabled = false;
    settings.single_process = false;
    CefString(&settings.cache_path) = strCacheDir;
    settings.remote_debugging_port = 9009;
    CefString(&settings.locale) = L"zh-CN";
    CefString(&settings.log_file) = strLogFile;
    settings.log_severity = LOGSEVERITY_ERROR;

#if defined(OS_WIN)
    settings.multi_threaded_message_loop =
        XGlobal::inst().CommandLine->HasSwitch(cefclient::kMultiThreadedMessageLoop);
#endif
}

std::wstring AppGetAppPath()
{
    CefString strCefAppPath;

    strCefAppPath = XGlobal::inst().CommandLine->GetCommandLineString();
    std::wstring strAppPath = strCefAppPath.ToWString();

    size_t pos = strAppPath.rfind(L'\\');
    if(pos != std::wstring::npos)
        strAppPath.resize(pos + 1);
    if(strAppPath.size() > 0 && strAppPath[0] == L'\"')
        strAppPath.erase(0, 1);

    strCefAppPath = strAppPath;

    return strCefAppPath;
}

void InitWebPlugins()
{
    std::wstring strNPSWF = AppGetAppPath();
    strNPSWF += L"NPSWF32_11_6_602_180.dll";
    CefAddWebPluginPath(strNPSWF);
    CefRefreshWebPlugins();
}
