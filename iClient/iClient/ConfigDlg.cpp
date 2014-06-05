#include "ConfigDlg.h"
#include "ui_ConfigDlg.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFontDialog>
#include <QColorDialog>
#include <QMessageBox>

ConfigDlg::ConfigDlg(USER_CONFIG& uc, SYSTEM_CONFIG& sc, bool l, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDlg),
    user_config(uc),
    system_config(sc),
    logined(l)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(300, 300);

    init_layout();
}

ConfigDlg::~ConfigDlg()
{
    delete ui;
}

void ConfigDlg::init_layout()
{
    ui->lb_server->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout* server_layout = new QHBoxLayout;
    server_layout->addWidget(ui->lb_server);
    server_layout->addWidget(ui->le_server);

    QHBoxLayout* btn_layout = new QHBoxLayout;
    btn_layout->addWidget(ui->pb_font_style);
    btn_layout->addWidget(ui->pb_font_color);
    btn_layout->addStretch();

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addStretch(1);
    main_layout->addWidget(ui->cb_sound);
    main_layout->addWidget(ui->cb_bubble);
    main_layout->addLayout(btn_layout);
    main_layout->addLayout(server_layout);
    main_layout->addStretch(9);
    main_layout->addWidget(ui->buttonBox);

    setLayout(main_layout);

    ui->cb_sound->setEnabled(logined);
    ui->cb_bubble->setEnabled(logined);
    ui->pb_font_style->setEnabled(logined);
    ui->pb_font_color->setEnabled(logined);

    ui->cb_sound->setChecked(user_config.sound);
    ui->cb_bubble->setChecked(user_config.bubble);
    ui->le_server->setText(system_config.server);
}

void ConfigDlg::accept()
{
    QString server = ui->le_server->text();
    if (server.isEmpty())
    {
        QMessageBox::warning(this, "提示", "请填写\"服务器\"信息");
        return ;
    }
    // 检验填写是否正确


    // save
    system_config.server = server;

    QDialog::accept();
}

void ConfigDlg::on_pb_font_style_clicked()
{
    QFontDialog fd(user_config.font_style);

    bool ok = false;
    user_config.font_style = fd.getFont(&ok);
}

void ConfigDlg::on_pb_font_color_clicked()
{
    QColorDialog cd(user_config.font_color);
    user_config.font_color = cd.getColor();
}

void ConfigDlg::on_cb_sound_clicked(bool checked)
{
    user_config.sound = checked;
}

void ConfigDlg::on_cb_bubble_clicked(bool checked)
{
    user_config.bubble = checked;
}
