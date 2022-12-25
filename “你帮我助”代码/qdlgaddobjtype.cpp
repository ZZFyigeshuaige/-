#include "qdlgaddobjtype.h"
#include "ui_qdlgaddobjtype.h"

QDlgAddObjType::QDlgAddObjType(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::QDlgAddObjType)
{
    ui->setupUi(this);
}

QDlgAddObjType::~QDlgAddObjType()
{
    delete ui;
}

//点击添加物品类型对话框的“添加新属性”按钮的槽函数
void QDlgAddObjType::on_btnAddObjTypeAdd_clicked()
{
    QString attributeName = QInputDialog::getText(this, "输入框", "请输入新的属性名");
    if (attributeName.isEmpty()) {
        QMessageBox::warning(this, "警告", "输入不可为空");
        return;
    }
    attributeNameList << attributeName;
}

//点击添加物品类型对话框的“确定”按钮的槽函数
void QDlgAddObjType::on_btnAddObjTypeOK_clicked()
{
    typeName = ui->editTypeName->text();
    if (typeName.isEmpty()) {
        QMessageBox::warning(this, "警告", "输入不可为空");
        return;
    }
    this->accept();
}

