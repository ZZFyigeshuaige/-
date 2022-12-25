#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QInputDialog>
#include <QFile>
#include <QMessageBox>

extern bool isUser; //全局变量，判定使用者是普通用户还是管理员

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddObjectType_clicked();
    void on_btnChangeObjectType_clicked();
    void on_btnAddObject_clicked();
    void on_btnSearchObject_clicked();
    void on_objectTree_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void on_btnDeleteObject_clicked();

    void on_btnUser_clicked();

private:
    Ui::MainWindow* ui;
    enum treeItemType { topItem = 1001, typeItem, objectItem };  //定义树的节点类型，从上至下分别为顶端节点，类型节点和物品节点
    QStringList typeNameList;   //字符串列表，储存所有物品类型的名称

    void iniTree();
    void addTypeItem(QString typeName, QStringList attributeNameList);
    void addObjectItem(QTreeWidgetItem* typeItem, QStringList attributeList);
    void deleteObjectItem();
    void searchObjectItem(QString typeName, QString keyWord);
    void showObjectItem(QTreeWidgetItem* item);
    void showInformation(QTreeWidgetItem* typeItem, QTreeWidgetItem* objectItem);
    void writeFile();
    void readFile();
    QTreeWidgetItem* searchTypeItem(QString typeName);
};

#endif // MAINWINDOW_H
