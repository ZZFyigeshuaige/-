#ifndef QDLGADDOBJTYPE_H
#define QDLGADDOBJTYPE_H

#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>

namespace Ui {
    class QDlgAddObjType;
}

class QDlgAddObjType : public QDialog
{
    Q_OBJECT

public:
    explicit QDlgAddObjType(QWidget* parent = nullptr);
    ~QDlgAddObjType();

    QString typeName;   //字符串，储存要添加的物品类型名称
    QStringList attributeNameList;  //字符串列表，储存要添加的类型属性名称

private slots:
    void on_btnAddObjTypeAdd_clicked();
    void on_btnAddObjTypeOK_clicked();

private:
    Ui::QDlgAddObjType* ui;
};

#endif // QDLGADDOBJTYPE_H
