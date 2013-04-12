#include "StdAfx.h"
#include "XUtil.h"


#include "client_switches.h"
#include "XGlobal.h"


// 初始化全局命令行对象
void AppInitCommandLine(int argc, const char* const* argv)
{
    XGlobal::inst().CommandLine = CefCommandLine::CreateCommandLine();
#if defined(OS_WIN)
    XGlobal::inst().CommandLine->InitFromString(::GetCommandLineW());
#else
    XGlobal::inst().CommandLine->InitFromArgv(argc, argv);
#endif
}


// 初始化Cef的配置
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

// 返回本程序所在的目录
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

    return strAppPath;
}

// 返回本程序的名字
std::wstring AppGetAppName()
{
    CefString strCefAppName;

    strCefAppName = XGlobal::inst().CommandLine->GetCommandLineString();
    std::wstring strAppName = strCefAppName.ToWString();

    size_t pos = strAppName.rfind(L'\\');
    if(pos == std::wstring::npos)
        strAppName.resize(0);
    else
        strAppName.erase(pos);

    return strAppName;
}

// 初始化插件，如Flash
void InitWebPlugins()
{
    std::wstring strNPSWF = AppGetAppPath();
    strNPSWF += L"NPSWF32_11_6_602_180.dll";
    CefAddWebPluginPath(strNPSWF);
    CefRefreshWebPlugins();
}

// 加载Exe中的资源
bool LoadBinaryResource(const wchar_t* type, CefString name, LPBYTE& pData, DWORD& dwSize)
{
    HINSTANCE hInst = ::GetModuleHandle(NULL);
    HRSRC hRes = FindResource(hInst, name.ToWString().c_str(), type);
    if (hRes)
    {
        HGLOBAL hGlob = LoadResource(hInst, hRes);
        if (hGlob)
        {
            dwSize = SizeofResource(hInst, hRes);
            pData = (LPBYTE)LockResource(hGlob);
            if(dwSize > 0 && pData != NULL)
            {
                return true;
            }
        }
    }
    return false;
}

bool LoadStringResource(const wchar_t* type, CefString name, CefString& strData)
{
    LPBYTE pData = NULL;
    DWORD dwSize = 0;
    if(LoadBinaryResource(type, name, pData, dwSize))
    {
        std::wstring str((const wchar_t*)pData, dwSize / 2);
        strData.FromWString(str.c_str());
        return true;
    }
    strData = "";
    return false;
}

CefRefPtr<CefStreamReader> GetBinaryResourceReader(const wchar_t* type, CefString name)
{
    LPBYTE pData = NULL;
    DWORD dwSize = 0;
    if(LoadBinaryResource(type, name, pData, dwSize))
    {
        return CefStreamReader::CreateForHandler(new CefByteReadHandler(pData, dwSize, NULL));
    }
    return NULL;
}