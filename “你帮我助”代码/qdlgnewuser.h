#ifndef QDLGNEWUSER_H
#define QDLGNEWUSER_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>

namespace Ui {
class QDlgNewUser;
}

class QDlgNewUser : public QDialog
{
    Q_OBJECT

public:
    explicit QDlgNewUser(QWidget *parent = nullptr);
    ~QDlgNewUser();

private slots:
    void on_btnAccept_clicked();
    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_btnReject_clicked();

private:
    Ui::QDlgNewUser *ui;
    int newRow;

    void iniTable();
};

#endif // QDLGNEWUSER_H
