#ifndef CONFIGDLG_H
#define CONFIGDLG_H

#include <QDialog>
#include "AppDef.h"

namespace Ui {
class ConfigDlg;
}

class ConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDlg(USER_CONFIG& uc, SYSTEM_CONFIG& sc, bool logined = false, QWidget *parent = 0);
    ~ConfigDlg();

private slots:
    void on_pb_font_style_clicked();
    void on_pb_font_color_clicked();

    void on_cb_sound_clicked(bool checked);

    void on_cb_bubble_clicked(bool checked);

public slots:
    virtual void accept();

private:
    Ui::ConfigDlg *ui;
    void init_layout();

    USER_CONFIG &user_config;
    SYSTEM_CONFIG &system_config;

    bool logined;
};

#endif // CONFIGDLG_H
