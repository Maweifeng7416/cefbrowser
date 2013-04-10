#include "StdAfx.h"
#include "XUtil.h"


#include "client_switches.h"

CefRefPtr<CefCommandLine> g_command_line;

void AppInitCommandLine(int argc, const char* const* argv)
{
    g_command_line = CefCommandLine::CreateCommandLine();
#if defined(OS_WIN)
    g_command_line->InitFromString(::GetCommandLineW());
#else
    g_command_line->InitFromArgv(argc, argv);
#endif
}




// Returns the application settings based on command line arguments.
void AppGetSettings(CefSettings& settings)
{
    ASSERT(g_command_line.get());
    if (!g_command_line.get())
        return;

    settings.single_process = false;
    CefString(&settings.cache_path) = L"E:\\Test\\CefBrowser";
    settings.remote_debugging_port = 9009;
    CefString(&settings.locale) = L"zh-CN";
    CefString(&settings.log_file) = L"E:\\Test\\CefBrowser\\CefBrowser.log";
    settings.log_severity = LOGSEVERITY_ERROR;

#if defined(OS_WIN)
    settings.multi_threaded_message_loop =
        g_command_line->HasSwitch(cefclient::kMultiThreadedMessageLoop);
#endif
}
