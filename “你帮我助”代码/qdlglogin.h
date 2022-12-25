#ifndef QDLGLOGIN_H
#define QDLGLOGIN_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>

extern bool isUser;

namespace Ui {
    class QDlgLogin;
}

class QDlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit QDlgLogin(QWidget* parent = nullptr);
    ~QDlgLogin();

private slots:
    void on_btnLoginOK_clicked();

private:
    Ui::QDlgLogin* ui;
};

#endif // QDLGLOGIN_H
