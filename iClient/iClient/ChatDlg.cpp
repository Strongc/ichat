#include "ChatDlg.h"
#include "ui_ChatDlg.h"
#include <QFontDialog>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>

ChatDlg::ChatDlg(USER_INFO &lusr, USER_INFO &tusr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDlg),
    local_user(lusr),
    target_user(tusr)
{
    ui->setupUi(this);
    setMinimumSize(580, 480);
    init_layout();
    ui->te_details->setReadOnly(true);

    ui->te_input->setTextColor(QColor(0,255,0));
    QString ss = QString(QStringLiteral("<font color=#871F78>测试文字</font>"));
    ui->te_details->append(ss);

    QString ddd = time_format.arg(local_user.user_name).arg(get_date_time());
    ui->te_details->append(ddd);
}

ChatDlg::~ChatDlg()
{
    delete ui;
}

void ChatDlg::init_layout()
{
    ui->te_details->setMinimumHeight(100);
    ui->te_input->setMinimumHeight(100);
    //ui->te_input->setGeometry();

    QSplitter* bottom_sp = new QSplitter(Qt::Vertical);
    bottom_sp->addWidget(ui->te_details);
    bottom_sp->addWidget(ui->te_input);

    QSplitter* main_sp = new QSplitter(Qt::Vertical);
    main_sp->addWidget(ui->gb_user);
    main_sp->addWidget(bottom_sp);
    main_sp->setStretchFactor(1, 1);

    QHBoxLayout* btn_layout = new QHBoxLayout;
    btn_layout->addStretch();
    btn_layout->addWidget(ui->pb_send);
    btn_layout->addWidget(ui->pb_close);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(main_sp);
    main_layout->addLayout(btn_layout);

    setLayout(main_layout);
}

int ChatDlg::get_user_id()
{
    return target_user.user_id;
}

void ChatDlg::on_pb_send_clicked()
{
    QString detail = time_format.arg(local_user.user_name).arg(get_date_time());
    ui->te_details->append(detail);
    QString input = ui->te_input->toHtml();
    ui->te_details->append(input);
}

QString ChatDlg::get_date_time()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    return current_date_time.toString("yyyy/MM/dd hh:mm:ss ddd");
}

void ChatDlg::add_chat_string(const QString &msg)
{
    QString detail = time_format.arg(target_user.user_name).arg(get_date_time());
    ui->te_details->append(detail);
    detail = target_user.uc.stylesheet.arg(msg);
    ui->te_details->append(detail);
}
