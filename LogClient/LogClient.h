#pragma once

#define XLOG_LEVEL_NONE         0x00
#define XLOG_LEVEL_DIAGNOSIS    0x01
#define XLOG_LEVEL_WARNING      0x02
#define XLOG_LEVEL_DEBUG        0x04
#define XLOG_LEVEL_ERROR        0x08
#define XLOG_LEVEL_FILE         0x10
#define XLOG_LEVEL_ALL          0xFF

// 打印Log
BOOL OutputLogImpl(UINT uLevel, LPCTSTR szFilter, LPCTSTR szFormat, ...);

// 设置接收服务器名字
void SetOutputServerImpl(LPCTSTR szName);

// 设置显示Log保存文件路径和级别
void SetOutputLogPathImpl(LPCTSTR szPath, DWORD dwLogLevel);

#if defined(_DEBUG) || defined(_X_OUTPUT_LOG_)

#define XSetOutputServer(name)                  SetOutputServerImpl(name)
#define XOutputLog(level, filter, format, ...)  OutputLogImpl((level),          (filter), (format), __VA_ARGS__)
#define XOutputLog1(filter, format, ...)        XOutputLog(XLOG_LEVEL_DIAGNOSIS,(filter), (format), __VA_ARGS__)
#define XOutputLog2(filter, format, ...)        XOutputLog(XLOG_LEVEL_WARNING,  (filter), (format), __VA_ARGS__)
#define XOutputLog3(filter, format, ...)        XOutputLog(XLOG_LEVEL_DEBUG,    (filter), (format), __VA_ARGS__)
#define XOutputLog4(filter, format, ...)        XOutputLog(XLOG_LEVEL_ERROR,    (filter), (format), __VA_ARGS__)
#define XOutputLog5(filter, format, ...)        XOutputLog(XLOG_LEVEL_FILE,     (filter), (format), __VA_ARGS__)

#define XSetOutputLogPath(path, level)          SetOutputLogPathImpl(path, level)

#else   // _DEBUG

#define XSetOutputServer(name)                  __noop
#define XOutputLog(level, filter, format, ...)  __noop
#define XOutputLog1(filter, format, ...)        __noop
#define XOutputLog2(filter, format, ...)        __noop
#define XOutputLog3(filter, format, ...)        __noop
#define XOutputLog4(filter, format, ...)        __noop
#define XOutputLog5(filter, format, ...)        __noop

#define XSetOutputLogPath(path, level)          __noop

#endif  // _DEBUG


// 链接库内容
#ifndef _NO_X_OUTPUT_LOG_

#ifdef  _DEBUG

    // Debug
    #if defined(_DLL)
        #pragma comment(lib, "dLogClient.lib")
    #else
        #pragma comment(lib, "sdLogClient.lib")
    #endif  // _X_STATIC_OUTOUT_LOG_

#else

    // Release
    #if defined(_DLL)
        #pragma comment(lib, "rLogClient.lib")
    #else
        #pragma comment(lib, "srLogClient.lib")
    #endif  // _X_STATIC_OUTOUT_LOG_

#endif  // _DEBUG


#endif  // _NO_X_OUTPUT_LOG_