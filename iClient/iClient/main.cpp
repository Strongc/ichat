#include "ChatClientDlg.h"
#include "LoginDlg.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 本地化
    QTranslator tran;
    tran.load("qt_zh_CN.qm", "lan\\");
    a.installTranslator(&tran);

    // 设置界面字体
    QFont font = a.font();
    font.setFamily(QString("微软雅黑"));
    font.setPointSize(10);
    a.setFont(font);

    LoginDlg w;
    w.show();

    return a.exec();
}
