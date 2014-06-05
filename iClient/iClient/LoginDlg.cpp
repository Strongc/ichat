#include "LoginDlg.h"
#include "ui_Login.h"
#include <QMessageBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QtXml/qdom.h>
#include <QTextStream>

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLoginDlg),
    chatclient_dlg(NULL)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(350, 180);

    init_layout();

    load_system_config();

// test data start
    ui->le_username->setText("1");
    ui->le_password->setText("1");
// test data end
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::init_layout()
{
    //ui->lb_config->setText(tr("<u><font color=blue>配置</u>"));
    ui->le_password->setEchoMode(QLineEdit::Password);
    ui->label_username->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->le_username->setMaxLength(10);
    ui->le_password->setMaxLength(10);

    QGridLayout* top_layout = new QGridLayout;
    top_layout->addWidget(ui->label_username, 0, 0);
    top_layout->addWidget(ui->le_username, 0, 1);
    top_layout->addWidget(ui->label_password, 1, 0);
    top_layout->addWidget(ui->le_password, 1, 1);

    QHBoxLayout* btn_layout = new QHBoxLayout;
    btn_layout->addStretch(1);
    btn_layout->addWidget(ui->pb_login);
    btn_layout->addWidget(ui->pb_close);
    btn_layout->addStretch();// 0:靠右, 1:居中

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addStretch();
    main_layout->addStretch();
    main_layout->addLayout(top_layout);
    main_layout->addLayout(btn_layout);
    main_layout->addStretch();
    setLayout(main_layout);
}

void LoginDlg::on_pb_login_clicked()
{
    if (ui->le_username->text().isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("\n   请输入用户名！\t\n"));
        ui->le_username->setFocus();
        return;
    }
    if (ui->le_password->text().isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("\n   请输入密码！\t\n"));
        ui->le_password->setFocus();
        return;
    }
    ui->le_username->clear();
    ui->le_password->clear();

    if (false == login())
    {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("\n   用户名或密码错误\t\n"));
        return;
    }

    chatclient_dlg = new ChatClientDlg(this);
    chatclient_dlg->show();
    hide();
}

void LoginDlg::on_pb_config_clicked()
{
    USER_CONFIG uc;
    uc.sound = false;
    uc.bubble = false;
    ConfigDlg config_dlg(uc, system_config, false);
    if (QDialog::Accepted == config_dlg.exec())
    {
        if (false == save_system_config())
        {
            QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("保存服务器信息失败"));
        }
    }
}

void LoginDlg::load_system_config()
{
    QFile file("./config/config.xml");
    if(false == file.open(QIODevice::ReadOnly))
    {
        system_config.server = QString("127.0.0.1:9696");
        return ;
    }

    QDomDocument doc;
    doc.setContent(&file);

    QDomElement system = doc.documentElement();// 加载根节点
    system_config.server = system.firstChildElement("server").text();
    file.close();
}

bool LoginDlg::save_system_config()
{
    QFile file("./config/config.xml");
    if(false == file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));
    QDomElement system = doc.createElement("system"); // 新建节点
    doc.appendChild(system);    // 加入节点
    QDomElement server = doc.createElement("server");
    server.appendChild(doc.createTextNode(system_config.server));   // 节点插入字符串
    system.appendChild(server);

    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    return true;
}


bool LoginDlg::login()
{
    return false;
}
