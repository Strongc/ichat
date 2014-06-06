#include "ChatClientDlg.h"
#include "ui_ChatClientDlg.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include "ConfigDlg.h"
#include "lib/url_client.h"

const QString main_control_ico("images/Main.ico");
const QString branck_ico("images/Branch.ico");
const QString user_head_ico("images/Header.ico");

enum
{
    NODE_CONTROL = 1,
    NODE_REGION  = 2,
    NODE_USER    = 3,
};

ChatClientDlg::ChatClientDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatClientDlg),
    timer(new QTimer),
    tray_icon(NULL),
    tray_menu(NULL)
{
    ui->setupUi(this);
    setMinimumSize(300, 600);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    // 布局
    init_layout();
    // 托盘图标
    init_tray();
    // 启动
    connect(timer, SIGNAL(timeout()), this, SLOT(start()));
    timer->start(300);
}

ChatClientDlg::~ChatClientDlg()
{
    timer->stop();
    foreach (ChatDlg* dlg, ls_chat_dlg)
    {
        delete dlg;
    }
    ls_chat_dlg.clear();

    delete ui;
}

void ChatClientDlg::init_layout()
{
    ui->tr_user_list->setColumnCount(1);
    QStringList headerLabels;
    headerLabels << QString(QStringLiteral("名称"));
    ui->tr_user_list->setHeaderLabels(headerLabels);
    ui->tr_user_list->setColumnWidth(0, 150);
    ui->tr_user_list->setStyleSheet("QTreeWidget::item{height:32px}");

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(ui->tr_user_list);
    setLayout(main_layout);
}

void ChatClientDlg::init_tray()
{
    tray_icon = new QSystemTrayIcon(QIcon(QString("images/tray.ico")), this);
    tray_icon->setToolTip(QStringLiteral("客户端"));
    tray_icon->show();
    tray_icon->showMessage(QStringLiteral("提示"), QStringLiteral("登录服务器成功"), QSystemTrayIcon::Information, 5000);
    connect(tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(tray_icon_activated(QSystemTrayIcon::ActivationReason)));

    tray_act_min = new QAction(QStringLiteral("最小化"), this);
    connect(tray_act_min, SIGNAL(triggered()), this, SLOT(hide()));
    tray_act_restore = new QAction(QStringLiteral("还原"), this);
    connect(tray_act_restore, SIGNAL(triggered()), this, SLOT(showNormal()));
    tray_act_config = new QAction(QStringLiteral("配置"), this);
    connect(tray_act_config, SIGNAL(triggered()), this, SLOT(config()));
    tray_act_quit = new QAction(QStringLiteral("退出"), this);
    connect(tray_act_quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    tray_menu = new QMenu(this);
    tray_menu->addAction(tray_act_min);
    tray_menu->addAction(tray_act_restore);
    tray_menu->addAction(tray_act_config);
    tray_menu->addSeparator();
    tray_menu->addAction(tray_act_quit);

    tray_icon->setContextMenu(tray_menu);
}

void ChatClientDlg::start()
{
    std::string region_list;
    int ret = get_all_region(region_list);
    if (ret)
    {

    }

    QList<ORG_INFO> org_list;
    ret = parse_region_list(region_list, org_list);
    if (ret)
    {

    }
    ORG_INFO oi;
    insert_org(oi);

    timer->stop();
}

void ChatClientDlg::insert_org(const ORG_INFO& org_info)
{
    tray_icon->showMessage(QStringLiteral("提示"), QStringLiteral("正在加载区域组织信息"), QSystemTrayIcon::Information, 5000);

    // test data start
    QTreeWidgetItem* twi = new QTreeWidgetItem(ui->tr_user_list, QStringList(QStringLiteral("浙江省XXX监控联网系统")));
    twi->setIcon(0, QIcon(main_control_ico));
    twi->setData(0, Qt::UserRole, 0+NODE_CONTROL);
    twi->setToolTip(0, QString(QStringLiteral("鼠标提示")));
    QTreeWidgetItem* twi1 = new QTreeWidgetItem(twi, QStringList(QStringLiteral("浙江省杭州西湖区分中心")));
    twi1->setIcon(0, QIcon(branck_ico));
    twi1->setData(0, Qt::UserRole, 10+NODE_REGION);
    QTreeWidgetItem* tw12 = new QTreeWidgetItem(twi1, QStringList(QStringLiteral("用户姓名1")));
    tw12->setIcon(0, QIcon(user_head_ico));
    tw12->setData(0, Qt::UserRole, 10+NODE_USER);
    tw12->setToolTip(0, QString(QStringLiteral("用户姓名1")));
    QTreeWidgetItem* tw13 = new QTreeWidgetItem(twi1, QStringList(QStringLiteral("用户姓名2")));
    tw13->setIcon(0, QIcon(user_head_ico));
    tw13->setData(0, Qt::UserRole, 20+NODE_USER);
    ui->tr_user_list->expandAll();
    // test data end

    QTreeWidgetItemIterator itr(ui->tr_user_list);
    while (*itr)
    {
        int item_id = (*itr)->data(0, Qt::UserRole).toInt() / 10;
        if (item_id == org_info.parent_org_id)
        {
            QTreeWidgetItem* twi = new QTreeWidgetItem(*itr, QStringList(org_info.org_name));
            twi->setIcon(0, QIcon(branck_ico));
            twi->setData(0, Qt::UserRole, org_info.org_id * 10 + NODE_REGION);

            break;
        }
        ++itr;
    }

    QMutexLocker locker(&ls_info_lock);
    ls_org_info.push_back(org_info);
}

void ChatClientDlg::insert_user(const USER_INFO& user_info)
{
    QTreeWidgetItemIterator itr(ui->tr_user_list);
    while (*itr)
    {
        int item_type = (*itr)->data(0, Qt::UserRole).toInt() % 10;
        int item_id = (*itr)->data(0, Qt::UserRole).toInt() / 10;
        if (item_type == NODE_REGION && item_id == user_info.parent_org_id)
        {
            QTreeWidgetItem* twi = new QTreeWidgetItem(*itr, QStringList(user_info.user_name));
            twi->setIcon(0, QIcon(user_head_ico));
            twi->setData(0, Qt::UserRole, user_info.user_id * 10 + NODE_USER);

            break;
        }
        ++itr;
    }

    QMutexLocker locker(&ls_info_lock);
    ls_user_info.push_back(user_info);
}

void ChatClientDlg::on_tr_user_list_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
    int item_type = item->data(0, Qt::UserRole).toInt() % 10;
    if (NODE_USER == item_type)
    {
        int item_id = item->data(0, Qt::UserRole).toInt() / 10;

        ChatDlg *dlg = get_chat_dlg(item_id);
        if (NULL != dlg)
        {
            dlg->show();
        }
    }
}

ChatDlg *ChatClientDlg::get_chat_dlg(const int user_id)
{
    QMutexLocker locker(&ls_dlg_lock);
    foreach (ChatDlg* dlg, ls_chat_dlg)
    {
        if (dlg->get_user_id() == user_id)
        {
            return dlg;
        }
    }

    USER_INFO usr;
    usr.user_id = user_id;
    usr.user_name = QString("jddd");
    ChatDlg *dlg = new ChatDlg(usr, usr, this);
    ls_chat_dlg.push_back(dlg);
    return dlg;
}

void ChatClientDlg::tray_icon_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:      // 单击托盘图标
    case QSystemTrayIcon::DoubleClick:  // 双击托盘图标
    {
        if (isVisible())
        {
            hide();
        }
        else
        {
            showNormal();
            raise();
        }
        break;
    }
    default:
        break;
    }
}

void ChatClientDlg::config()
{
    SYSTEM_CONFIG sc;
    ConfigDlg config_dlg(local_user_info.uc, sc, true);
    if (QDialog::Accepted == config_dlg.exec())
    {
        //if (false == save_system_config())
        {
            QMessageBox::warning(this, "提示", "保存服务器信息失败");
        }
    }
}

int ChatClientDlg::get_all_region(std::string &region_list)
{
    return get_url_client().get_("http://10.20.35.20:6262/api/region", region_list);
}
