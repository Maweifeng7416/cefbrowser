#include "StdAfx.h"
#include "XGlobal.h"

XGlobal::XGlobal(void)
{
    ClientHandler = new XClientHandler;
}

XGlobal::~XGlobal(void)
{
}

XGlobal& XGlobal::inst()
{
    static XGlobal inst;
    return inst;
}

