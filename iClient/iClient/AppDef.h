#ifndef APPDEF_H
#define APPDEF_H
#include <QFont>
#include <QColor>

static QString time_format("<font color=blue size=10>%1 %2</font>");

typedef struct tagUserConfig
{
    bool sound;
    bool bubble;
    QFont font_style;
    QColor font_color;

    QString stylesheet;
}USER_CONFIG;

typedef struct tagSystemConfig
{
    QString server;
}SYSTEM_CONFIG;

typedef struct tagUserInfo
{
    int user_id;
    QString user_name;
    QString user_ip;
    int parent_org_id;
    QString user_org;

    USER_CONFIG uc;
}USER_INFO;

typedef struct tagOrgInfo
{
    int org_id;
    QString org_name;
    int parent_org_id;
}ORG_INFO;

#endif // APPDEF_H
