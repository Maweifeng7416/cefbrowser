#pragma once


template < int DEFAULT_BUFFER_SIZE = 1024 >
class CBuffer
{
    CBuffer(const CBuffer&);
    CBuffer& operator = (const CBuffer&);
public:
    CBuffer()
    {
        m_pBuffer = NULL;
        m_dwBufferSize = 0;
    }
    ~CBuffer()
    {
        Release();
    }

public:
    void* GetBuffer(DWORD dwLength)
    {
        if(dwLength <= DEFAULT_BUFFER_SIZE)
        {
            return m_DefBuffer;
        }
        else if(dwLength <= m_dwBufferSize)
        {
            return m_pBuffer;
        }
        else
        {
            Release();
            m_pBuffer = malloc(dwLength);
            m_dwBufferSize = dwLength;
            return m_pBuffer;
        }
    }

private:
    void Release()
    {
        if(m_pBuffer != NULL)
        {
            free(m_pBuffer);
            m_pBuffer = NULL;
            m_dwBufferSize = 0;
        }
    }

private:
    DWORD   m_dwBufferSize;
    void*   m_pBuffer;
    BYTE    m_DefBuffer[DEFAULT_BUFFER_SIZE];
};