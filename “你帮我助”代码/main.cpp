#include "mainwindow.h"
#include "qdlglogin.h"
#include <QApplication>

bool isUser;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QDlgLogin* dlgLogin = new QDlgLogin;
    if (dlgLogin->exec() == QDialog::Accepted)  //如果登录成功，就显示主功能界面
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else return 0;
}
