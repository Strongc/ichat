void on_pb_config_clicked();
#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include "ChatClientDlg.h"
#include "ConfigDlg.h"
#include "AppDef.h"
#include "lib/url_client.h"

namespace Ui {
class QLoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();

private:
    Ui::QLoginDlg *ui;

    void init_layout();

    ChatClientDlg *chatclient_dlg;

    SYSTEM_CONFIG system_config;

    void load_system_config();
    bool save_system_config();

    bool login();

private slots:
    void on_pb_login_clicked();
    void on_pb_config_clicked();
};

#endif // LOGINDLG_H
