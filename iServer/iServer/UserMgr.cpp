#include "StdAfx.h"
#include "UserMgr.h"
#include "NetProc.h"

CUserMgr& GetUserMgr()
{
    static CUserMgr um;
    return um;
}

CUserMgr::CUserMgr(void)
{
    m_userOper.OpenDb();
    LoadRes();
    m_thSyncRes.Start();
}

CUserMgr::~CUserMgr(void)
{
    m_SyncEvent.notify_one();
    m_thSyncRes.Stop();
    m_userOper.CloseDb();
}


// 从数据库获取资源
bool CUserMgr::LoadRes(void)
{
    CBoostGuard regLock(&m_regionLock);
    m_lsAllRegion.clear();
    if (false == m_userOper.GetRegions(m_lsAllRegion))
    {
        return false;
    }

    CBoostGuard usrLock(&m_userLock);
    m_lsAllUser.clear();
    if (false == m_userOper.GetUsers(m_lsAllUser))
    {
        return false;
    }

    printfd("get user size:%d\n", m_lsAllUser.size());

    return true;
}
// 处理心跳消息
bool CUserMgr::UserHeartbeat(const int nUserId)
{
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == nUserId)
        {
            itr->set_last_beat(time(NULL));
            printfd("user %d heart beat %d\n", nUserId, (int)time(NULL));
            if (time(NULL) % 5 == 0)    // 5s刷新一次
            {
                m_userOper.UpdateUserInfo(*itr);
            }
            break;
        }
    }
    return true;
}

// 验证用户登录
bool CUserMgr::UserLogin(const LoginInfo& loginInfo, LoginResult* pLoginResult)
{
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_name() == loginInfo.user_name())
        {
            if (itr->user_pwd() != loginInfo.user_pwd())
            {
                pLoginResult->set_user_id(-1);
                pLoginResult->set_desc("wrong password!");
                return false;
            }
            else
            {
                pLoginResult->set_user_id(itr->user_id());
                itr->set_longin_time(time(NULL));
                itr->set_user_ip(loginInfo.user_ip());
                m_userOper.UpdateUserInfo(*itr);
                return true;
            }
        }
    }

    pLoginResult->set_user_id(-1);
    pLoginResult->set_desc("no such user!");
    return false;
}

// 用户登出处理
bool CUserMgr::UserLogout(const int nUserId)
{
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == nUserId)
        {
            itr->set_logout_time(time(NULL));
            m_userOper.UpdateUserInfo(*itr);
            return true;
        }
    }

    return false;
}

// 获取用户配置
bool CUserMgr::GetUserConfig(const int nUserId, UserConfig* pUserConfig)
{
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == nUserId)
        {
            pUserConfig->CopyFrom(itr->user_config());
            return true;
        }
    }

    return false;
}

// 设置用户配置
bool CUserMgr::SetUserConfig(const int nUserId, const UserConfig& userConfig)
{
    if (false == m_userOper.UpdateUserConfig(nUserId, userConfig))
    {
        return false;
    }
    CBoostGuard usrLock(&m_userLock);
    for (std::list<User>::iterator itr = m_lsAllUser.begin();
        itr != m_lsAllUser.end(); ++itr)
    {
        if (itr->user_id() == nUserId)
        {
            itr->mutable_user_config()->CopyFrom(userConfig);
            return true;
        }
    }
    return true;
}

// 获取区域结构
bool CUserMgr::GetRegion(const char* client_ip)
{
    TransMsg transMsg;
    transMsg.set_cmd(CLIENT_GET_REGION_RSP);
    transMsg.set_result(0);
    CBoostGuard regLock(&m_regionLock);
    for (std::list<Region>::const_iterator itr = m_lsAllRegion.begin();
        itr != m_lsAllRegion.end(); ++itr)
    {
        transMsg.add_region()->CopyFrom(*itr);
    }
    int tmp = 0;
    string strbuf;
    transMsg.SerializeToString(&strbuf);
    int nRet = GetNetProc().SendMsg(client_ip, 7321, strbuf.c_str(), strbuf.length(), NULL, tmp);
    printfd("region msg(%d):\n%s\n", nRet, transMsg.DebugString().c_str());

    return (nRet>0)?true:false;
}

// 根据区域ID获取下属的用户
bool CUserMgr::GetUserByRegion(const char* client_ip, const int nRegionId)
{
    int nUserCount = 0;
    string strbuf;

    TransMsg transMsg;
    transMsg.set_cmd(CLIENT_GET_USER_RSP);
    transMsg.set_result(0);
    CBoostGuard usrLock(&m_userLock);
    std::list<User>::const_iterator itr = m_lsAllUser.begin();
    while (itr != m_lsAllUser.end())
    {
        if (itr->parent_id() != nRegionId)
        {
            continue;
        }

        int tmp = 0;
        transMsg.add_user()->CopyFrom(*itr);
        ++nUserCount;
        if (nUserCount == 5 || itr == m_lsAllUser.end())    // 发送一次
        {
            nUserCount = 0;
            transMsg.SerializeToString(&strbuf);
            int nRet = GetNetProc().SendMsg(client_ip, 7321, strbuf.c_str(), strbuf.length(), NULL, tmp);
            printfd("region msg...(%d):\n%s\n", nRet, transMsg.DebugString().c_str());
            transMsg.clear_user();
        }
    }

    return true;
}

// 根据用户ID获取用户信息
bool CUserMgr::GetUser(const int nUserId, User& userInfo)
{
    return false;
}
