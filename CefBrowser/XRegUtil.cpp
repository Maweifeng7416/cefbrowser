#include "stdafx.h"
#include "XRegUtil.h"

#include "XBuffer.h"


CRegUtil::CRegUtil()
{
    m_hKey = NULL;
}

CRegUtil::~CRegUtil()
{
    Close();
}

bool CRegUtil::Open(HKEY hParentKey, const wchar_t * const path, RegMode mode)
{
    Close();

    REGSAM sam = KEY_READ;
    switch(mode)
    {
    default:
    case RM_ReadOnly:
        sam = KEY_READ;
        break;
    case RM_WriteOnly:
        sam = KEY_WRITE;
        break;
    case RM_ReadAndWrite:
        sam = KEY_READ | KEY_WRITE;
        break;
    }
    LONG lResult = ::RegOpenKeyEx(hParentKey, path, 0, sam, &m_hKey);
    bool bResult = (lResult == ERROR_SUCCESS && m_hKey != NULL);
    if(!bResult)
    {
        int a = 0;
    }
    return bResult;
}

void CRegUtil::Close()
{
    if(m_hKey != NULL)
    {
        ::RegCloseKey(m_hKey);
        m_hKey = NULL;
    }
}

bool CRegUtil::GetValue(const wchar_t* const name, std::wstring& strValue)
{
    const int nSize = 1024;
    CBuffer<nSize> buffer;
    DWORD dwLength = 0;
    DWORD dwType = REG_NONE;

    void* pBuffer = buffer.GetBuffer(nSize);
    LONG lResult = ::RegQueryValueEx(m_hKey, name, 0, &dwType, (LPBYTE)pBuffer, &dwLength);
    if((lResult != ERROR_MORE_DATA && lResult != ERROR_SUCCESS)
        || (dwType != REG_SZ)
        || dwLength == 0)
    {
        return false;
    }

    if(lResult == ERROR_MORE_DATA)
    {
        pBuffer = buffer.GetBuffer(dwLength);
        lResult = ::RegQueryValueEx(m_hKey, name, 0, &dwType, (LPBYTE)pBuffer, &dwLength);
    }

    if(lResult != ERROR_SUCCESS)
        return false;

    strValue.assign((const wchar_t*)pBuffer, dwLength / sizeof(wchar_t));
    if(strValue.at(strValue.size() - 1) == 0)
        strValue.resize(strValue.size() - 1);
    return true;
}
