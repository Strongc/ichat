#ifndef CHATCLIENTDLG_H
#define CHATCLIENTDLG_H

#include <QDialog>
#include "ChatDlg.h"
#include "AppDef.h"
#include <QTreeWidget>
#include <QMutexLocker>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

namespace Ui {
class ChatClientDlg;
}

class ChatClientDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChatClientDlg(QWidget *parent = 0);
    ~ChatClientDlg();

private slots:
    void on_tr_user_list_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void start();
    void tray_icon_activated(QSystemTrayIcon::ActivationReason reason);
    void config();
private:
    Ui::ChatClientDlg *ui;
    QTimer *timer;

    void init_layout();

    QSystemTrayIcon* tray_icon;
    QAction *tray_act_min;
    QAction *tray_act_restore;
    QAction *tray_act_config;
    QAction *tray_act_quit;
    QMenu* tray_menu;

    QMutex ls_dlg_lock;
    QList<ChatDlg*> ls_chat_dlg;

    QMutex ls_info_lock;
    QList<ORG_INFO> ls_org_info;
    QList<USER_INFO> ls_user_info;

    void insert_org(const ORG_INFO& org_info);
    void insert_user(const USER_INFO& user_info);

    USER_INFO local_user_info;

    ChatDlg *get_chat_dlg(const int user_id);

    void init_tray();
    //void flush_tray_icon();

    int get_all_region(std::string& region_list);
    int parse_region_list(std::string& region_list, QList<ORG_INFO>& org_list);
};

#endif // CHATCLIENTDLG_H
