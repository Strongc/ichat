#ifndef CHATDLG_H
#define CHATDLG_H

#include <QDialog>
#include "AppDef.h"

namespace Ui {
class ChatDlg;
}

class ChatDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDlg(USER_INFO &local_user, USER_INFO &target_user, QWidget *parent = 0);
    ~ChatDlg();

private:
    Ui::ChatDlg *ui;

    void init_layout();

    USER_INFO &local_user;
    USER_INFO &target_user;

    QString get_date_time();
public:
    int get_user_id();
    void add_chat_string(const QString& msg);
private slots:
    void on_pb_send_clicked();
};

#endif // CHATDLG_H
