#include "qdlglogin.h"
#include "ui_qdlglogin.h"

QDlgLogin::QDlgLogin(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::QDlgLogin)
{
    ui->setupUi(this);
    ui->editPassword->setEchoMode(QLineEdit::Password);     //使密码输入隐藏
    this->setAttribute(Qt::WA_DeleteOnClose);   //设定该窗口在关闭后释放其资源
}

QDlgLogin::~QDlgLogin()
{
    delete ui;
}

void QDlgLogin::on_btnLoginOK_clicked()
{
    QSettings userSettings("WWB-Qt", "user");    //普通用户注册表，按键-键值方式储存账号-密码
    QSettings adminSettings("WWB-Qt", "admin");  //管理员注册表，按键-键值方式储存账号-密码
    QSettings newUserSettings("WWB-Qt","newUser");  //新用户注册表
    QString account = ui->editAccount->text();    //读取输入的账号
    QString password = ui->editPassword->text();  //读取输入的密码
    adminSettings.setValue("0","0");

    if (account.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this,"警告","输入不可为空");
        return;
    }

    //如果选中用户注册按钮
    if (ui->btnUserRegister->isChecked()) {
        if (userSettings.contains(account)) QMessageBox::warning(this, "提示", "账号已存在，请重新注册");
        else {
            newUserSettings.setValue(account, password);
            QMessageBox::information(this, "提示", "用户注册成功，请等待管理员通过");
        }
    }

    //如果选中管理员登录按钮
    if (ui->btnAdminLogin->isChecked()) {
        isUser = false;
        if (adminSettings.contains(account)) {
            if (adminSettings.value(account).toString() == password) this->accept();
            else QMessageBox::warning(this, "错误", "账号或密码错误");
        }
        else QMessageBox::warning(this, "错误", "账号或密码错误");
    }

    //如果选中用户登录按钮
    if (ui->btnUserLogin->isChecked()) {
        isUser = true;
        if (userSettings.contains(account)) {
            if (userSettings.value(account).toString() == password) this->accept();
            else QMessageBox::warning(this, "错误", "账号或密码错误");
        }
        else QMessageBox::warning(this, "错误", "账号或密码错误");
    }
}

