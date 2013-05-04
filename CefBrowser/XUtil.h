#pragma once




// 初始化全局命令行对象
void AppInitCommandLine(int argc, const char* const* argv);


// 初始化Cef的配置
void AppGetSettings(CefSettings& settings);

// 返回本程序所在的目录
std::wstring AppGetAppPath();
// 返回本程序的名字
std::wstring AppGetAppName();

// 初始化插件，如Flash
void InitWebPlugins();

// 加载Exe中的资源
bool LoadStringResource(const wchar_t* type, CefString name, CefString& strData);
CefRefPtr<CefStreamReader> GetBinaryResourceReader(const wchar_t* type, CefString name);


void ReplaceToken(std::wstring& str, std::wstring& oldToken, std::wstring& newToken);

