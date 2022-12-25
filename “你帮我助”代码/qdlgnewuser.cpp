#include "qdlgnewuser.h"
#include "ui_qdlgnewuser.h"

QDlgNewUser::QDlgNewUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDlgNewUser)
{
    ui->setupUi(this);
    iniTable();
}

QDlgNewUser::~QDlgNewUser()
{
    delete ui;
}

void QDlgNewUser::iniTable(){

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    newRow=-1;

    QSettings newUserSettings("WWB-Qt","newUser");
    QStringList keyList=newUserSettings.childKeys();
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(keyList.length());

    QTableWidgetItem* item;
    item = new QTableWidgetItem;
    item->setText("账号");
    ui->tableWidget->setHorizontalHeaderItem(0,item);
    item = new QTableWidgetItem;
    item->setText("密码");
    ui->tableWidget->setHorizontalHeaderItem(1,item);

    for (int i=0;i<keyList.length();i++){
        item=new QTableWidgetItem;
        item->setText(keyList[i]);
        ui->tableWidget->setItem(i,0,item);
        item=new QTableWidgetItem;
        item->setText(newUserSettings.value(keyList[i]).toString());
        ui->tableWidget->setItem(i,1,item);
    }
}

void QDlgNewUser::on_btnAccept_clicked()
{
    if (newRow==-1){
        QMessageBox::warning(this,"警告","未选择对象");
        return;
    }

    QSettings userSettings("WWB-Qt", "user");
    QSettings newUserSettings("WWB-Qt","newUser");
    QTableWidgetItem* item=ui->tableWidget->item(newRow,0);
    userSettings.setValue(item->text(),ui->tableWidget->item(newRow,1)->text());
    newUserSettings.remove(item->text());
    ui->tableWidget->removeRow(newRow);
}


void QDlgNewUser::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    newRow=currentRow;
}


void QDlgNewUser::on_btnReject_clicked()
{
    if (newRow==-1){
        QMessageBox::warning(this,"警告","未选择对象");
        return;
    }

    QSettings newUserSettings("WWB-Qt","newUser");
    QTableWidgetItem* item=ui->tableWidget->item(newRow,0);
    newUserSettings.remove(item->text());
    ui->tableWidget->removeRow(newRow);
}

