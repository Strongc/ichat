// iServer.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "iServer.h"

#include "UserMgr.h"
#include "NetProc.h"

ISERVER_API int fniServer(void)
{
    GetUserMgr().LoadRes();
    return 0;
}


