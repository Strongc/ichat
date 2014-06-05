#pragma once
#include "UserOperator.h"

class CUserMgr;
CUserMgr& GetUserMgr();

class CUserMgr: public boost::noncopyable
{
public:
    CUserMgr(void);
    ~CUserMgr(void);
    // 从数据库获取资源：用户列表、区域列表、用户区域组织列表
    bool LoadRes(void);

private:
    CBoostEvent m_SyncEvent;
    CUserOperator m_userOper;

    CBoostLock m_regionLock;
    std::list<Region> m_lsAllRegion;
    CBoostLock m_userLock;
    std::list<User> m_lsAllUser;
private:
    class CSyncThread
    {
    public:
        void Start()
        {
            m_thread.reset(new boost::thread(boost::bind(&CSyncThread::SyncRes, this)));
        }
        void Stop()
        {
            m_thread->interrupt();
        }

    private:
        boost::scoped_ptr<boost::thread> m_thread;
        void SyncRes()
        {
            while(false == GetUserMgr().m_SyncEvent.timed_wait(5*1000))
            {
                GetUserMgr().LoadRes();
            }
        }
    };

    CSyncThread m_thSyncRes;

public:
    // 验证用户登录
    bool UserLogin(const LoginInfo& loginInfo, LoginResult* pLoginResult);
    // 用户登出处理
    bool UserLogout(const int nUserId);
    // 处理心跳消息
    bool UserHeartbeat(const int nUserId);
    // 获取用户配置
    bool GetUserConfig(const int nUserId, UserConfig* pUserConfig);
    // 设置用户配置
    bool SetUserConfig(const int nUserId, const UserConfig& userConfig);
    // 获取区域结构
    bool GetRegion(const char* client_ip);
    // 根据区域ID获取下属的用户
    bool GetUserByRegion(const char* client_ip, const int nRegionId);
    // 根据用户ID获取用户信息
    bool GetUser(const int nUserId, User& userInfo);
};




