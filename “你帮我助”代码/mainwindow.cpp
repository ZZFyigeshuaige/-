#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qdlgaddobjtype.h"
#include "qdlgnewuser.h"

//构造函数，调用了自定义的物品树的初始化函数iniTree()
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iniTree();

    //若使用者为普通用户，添加物品类型和修改物品类型操作不可见
    if (isUser == true) {
        ui->btnAddObjectType->setVisible(false);
        ui->btnChangeObjectType->setVisible(false);
        ui->btnUser->setVisible(false);
    }

    if (isUser == false) {
        ui->btnAddObject->setVisible(false);
        ui->btnDeleteObject->setVisible(false);
        ui->btnSearchObject->setVisible(false);
    }
}

//析构函数，调用writeFile()函数将物品树中数据保存
MainWindow::~MainWindow()
{

    writeFile();
    delete ui;

}

//物品树的初始化函数，进行顶层节点的创建。调用readFile()函数，进行数据的读入
void MainWindow::iniTree() {
    ui->objectTree->clear();    //清空树
    QTreeWidgetItem* item = new QTreeWidgetItem(MainWindow::topItem);
    item->setText(0, "物品");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    ui->objectTree->addTopLevelItem(item);  //添加顶层节点，显示文本为“物品”，属性为可选中、自动折叠
    typeNameList.clear();
    readFile();     //读入数据
}

//从txt文件中读取物品数据，并添加到物品树中
void MainWindow::readFile() {
    //打开文件
    QFile aFile("D:/desktop/help.txt");
    if (!aFile.exists()) {
        QMessageBox::warning(this, "警告", "文件不存在");
        return;
    }
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", "文件读取失败");
        return;
    }

    QTextStream aStream(&aFile);    //以流方式从文件中读取数据
    aStream.setAutoDetectUnicode(true);     //防止出现乱码
    QString str;    //字符串，储存每次读取出的数据
    QStringList list;   //字符串列表，储存物品类型的属性名称或是物品的信息

    if (!aStream.atEnd()) str = aStream.readLine();
    while (!aStream.atEnd()) {
        //如果读取到字符串typeItem，说明接下来一段内是某个物品类型，读取其中内容并添加一个类型节点至物品树中
        if (str == "typeItem") {
            QTreeWidgetItem* typeItem = new QTreeWidgetItem(MainWindow::typeItem);
            str = aStream.readLine();     //typeItem的下一行是类型名称，接下来的几行是类型的属性名称
            typeItem->setText(0, str);
            typeNameList << str;
            list.clear();

            //将所有属性名称读取到list中
            while (!aStream.atEnd()) {
                str = aStream.readLine();
                if (str != "typeItem" && str != "objectItem") list << str;
                else break;
            }

            typeItem->setData(0, Qt::UserRole, list);
            ui->objectTree->topLevelItem(0)->addChild(typeItem);    //添加类型节点
        }

        //如果读取到字符串objectType，说明接下来一段内是某个物品，读取其中内容并添加一个物品节点至物品树中
        if (str == "objectItem") {
            QTreeWidgetItem* objectItem = new QTreeWidgetItem(MainWindow::objectItem);
            list.clear();

            //读取物品信息，for循环读取公共信息，while循环读取每个物品类型特有的属性
            for (int i = 0; i < 5; i++) list << aStream.readLine();
            while (!aStream.atEnd()) {
                str = aStream.readLine();
                if (str != "typeItem" && str != "objectItem") list << str;
                else break;
            }

            objectItem->setText(0, list[0]);
            objectItem->setData(0, Qt::UserRole, list);
            ui->objectTree->topLevelItem(0)->child(ui->objectTree->topLevelItem(0)
                ->childCount() - 1)->addChild(objectItem);  //添加物品节点至最后一个类型节点
        }
    }
    aFile.close();      //关闭文件
}

//将物品树数据保存至txt文件中
void MainWindow::writeFile() {
    QFile aFile("D:/desktop/help.txt");
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", "文件保存失败");
        return;
    }
    QTextStream aStream(&aFile);
    aStream.setAutoDetectUnicode(true);
    QTreeWidgetItem* typeItem, * objectItem;

    //遍历所有类型节点
    for (int i = 0; i < ui->objectTree->topLevelItem(0)->childCount(); i++) {
        typeItem = ui->objectTree->topLevelItem(0)->child(i);    //指向当前类型节点
        aStream << "typeItem\n";
        aStream << typeItem->text(0) << "\n";   //输出类型名称
        for (int j = 0; j < typeItem->data(0, Qt::UserRole).toStringList().length(); j++)
            aStream << typeItem->data(0, Qt::UserRole).toStringList()[j] << "\n";    //输出属性名称

        //遍历当前类型节点的所有物品节点
        for (int k = 0; k < typeItem->childCount(); k++) {
            objectItem = typeItem->child(k);      //指向当前物品节点
            aStream << "objectItem\n";
            for (int l = 0; l < objectItem->data(0, Qt::UserRole).toStringList().length(); l++)
                aStream << objectItem->data(0, Qt::UserRole).toStringList()[l] << "\n";  //输出物品信息
        }
    }

    aFile.close();
}

//添加物品类型函数，传递参数为类型名称和属性名称列表，在物品树中添加类型节点
void MainWindow::addTypeItem(QString typeName, QStringList attributeNameList) {
    QTreeWidgetItem* item = new QTreeWidgetItem(MainWindow::typeItem);    //创建类型节点
    item->setText(0, typeName);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setData(0, Qt::UserRole, QVariant(attributeNameList));
    ui->objectTree->topLevelItem(0)->addChild(item);    //添加类型节点
    typeNameList << typeName;     //在type中添加新类型名称
}

//点击主功能窗口的“添加物品类型”按钮的槽函数
void MainWindow::on_btnAddObjectType_clicked()
{
    QDlgAddObjType* dialog = new QDlgAddObjType;      //创建添加物品类型对话框
    if (dialog->exec() == QDialog::Accepted)    //如果添加成功，调用addTypeItem()函数在物品树中添加类型节点
        addTypeItem(dialog->typeName, dialog->attributeNameList);
}

//功能函数，根据传递的类型名称，查找并返回对应的类型节点
QTreeWidgetItem* MainWindow::searchTypeItem(QString typeName) {
    for (int i = 0; i < ui->objectTree->topLevelItem(0)->childCount(); i++) {
        if (typeName == ui->objectTree->topLevelItem(0)->child(i)->text(0))
            return ui->objectTree->topLevelItem(0)->child(i);
    }
    return NULL;
}

//点击主功能窗口的“修改物品类型”按钮的槽函数
void MainWindow::on_btnChangeObjectType_clicked()
{
    QString oldTypeName = QInputDialog::getItem(this, "选择", "请选择要修改的物品类型", typeNameList, 0, false);    //弹出对话框，接收要修改的类型名称
    QString newAttributeName, newTypeName;
    QStringList attributeNameList;
    QTreeWidgetItem* typeItem = searchTypeItem(oldTypeName);

    newTypeName = QInputDialog::getText(this, "输入", "请输入新的类型名", QLineEdit::Normal, typeItem->text(0));   //弹出对话框，接收新类型名称
    if (newTypeName.isEmpty()) {
        QMessageBox::warning(this, "警告", "输入不可为空");
        return;
    }

    //依次弹出对话框，接收新属性名称
    for (int i = 0; i < typeItem->data(0, Qt::UserRole).toStringList().length(); i++) {
        newAttributeName = QInputDialog::getText(this, "输入", "请输入新的属性名", QLineEdit::Normal, typeItem->data(0, Qt::UserRole).toStringList()[i]);
        if (newAttributeName.isEmpty()) {
            QMessageBox::warning(this, "警告", "添加失败");
            return;
        }
        attributeNameList << newAttributeName;
    }

    typeItem->setText(0, newTypeName);
    typeNameList.replace(typeNameList.indexOf(oldTypeName), newTypeName);   //在typeNameList中相应修改类型名称
    typeItem->setData(0, Qt::UserRole, attributeNameList);
}

//点击主功能窗口的“添加物品”按钮的槽函数
void MainWindow::on_btnAddObject_clicked()
{
    QString typeName = QInputDialog::getItem(this, "选择", "请选择物品类型", typeNameList, 0, false);
    QTreeWidgetItem* typeItem = searchTypeItem(typeName);
    QStringList attributeList;
    attributeList << QInputDialog::getText(this, "输入", "请输入物品名称");
    attributeList << QInputDialog::getText(this, "输入", "请输入物品说明");
    attributeList << QInputDialog::getText(this, "输入", "请输入物品所在地址");
    attributeList << QInputDialog::getText(this, "输入", "请输入手机号码");
    attributeList << QInputDialog::getText(this, "输入", "请输入邮箱");
    for (int i = 0; i < typeItem->data(0, Qt::UserRole).toStringList().length(); i++)
        attributeList << QInputDialog::getText(this, "输入", "请输入" + typeItem->data(0, Qt::UserRole).toStringList()[i]);

    if (attributeList.contains("")) {
        QMessageBox::warning(this, "警告", "添加失败");
        return;
    }
    addObjectItem(typeItem, attributeList);  //调用addObjectItem()在物品树中添加物品节点
}

//添加物品函数，传递参数为所属类型节点和物品信息列表
void MainWindow::addObjectItem(QTreeWidgetItem* typeItem, QStringList attributeList) {
    QTreeWidgetItem* objectItem = new QTreeWidgetItem(MainWindow::objectItem);
    objectItem->setText(0, attributeList[0]);
    objectItem->setData(0, Qt::UserRole, attributeList);
    typeItem->addChild(objectItem);
}

//点击主功能窗口的“查找物品”按钮的槽函数
void MainWindow::on_btnSearchObject_clicked()
{
    QString typeName = QInputDialog::getItem(this, "选择", "请选择物品类型", typeNameList, 0, false);
    QString keyWord = QInputDialog::getText(this, "输入", "请输入查找关键字");
    if (keyWord.isEmpty()) {
        QMessageBox::warning(this, "警告", "输入不可为空");
        return;
    }
    searchObjectItem(typeName, keyWord);     //调用searchObjectItem()输出结果
}

//查找物品函数，传递参数为类型名称和关键字，在主功能窗口的文本框中显示结果
void MainWindow::searchObjectItem(QString typeName, QString keyWord) {
    QList<QTreeWidgetItem*> itemList;      //储存所有符合要求的物品节点
    QTreeWidgetItem* typeItem = searchTypeItem(typeName);
    int result = 0;

    //遍历该类型节点所有的物品节点
    for (int i = 0; i < typeItem->childCount(); i++) {
        //将关键字与物品名称和物品说明进行比对
        if (typeItem->child(i)->data(0, Qt::UserRole).toStringList()[0].contains(keyWord)
            || typeItem->child(i)->data(0, Qt::UserRole).toStringList()[1].contains(keyWord))
        {
            result++;
            itemList.append(typeItem->child(i));
        }
    }

    if (result == 0) QMessageBox::information(this, "消息", "未找到结果");
    else {
        QMessageBox::information(this, "消息", "共找到" + QString::number(result) + "条结果，请在文本框中查看");
        ui->plainTextEdit->clear();

        //在文本框中输出结果
        ui->plainTextEdit->appendPlainText("共找到" + QString::number(result) + "条结果：");
        for (int i = 0; i < result; i++) {
            showInformation(typeItem, itemList[i]);     //调用showInformation()显示物品的信息
            ui->plainTextEdit->appendPlainText("\n");
        }
    }
}

//功能函数，输出物品的所有信息，传递参数为物品节点及其所在类型节点
void MainWindow::showInformation(QTreeWidgetItem* typeItem, QTreeWidgetItem* objectItem) {
    QStringList attributeList = objectItem->data(0, Qt::UserRole).toStringList();      //储存物品信息的列表
    QStringList attributeNameList = typeItem->data(0, Qt::UserRole).toStringList();    //储存类型属性名称的列表
    ui->plainTextEdit->appendPlainText("物品名称：" + attributeList[0]);
    ui->plainTextEdit->appendPlainText("物品说明：" + attributeList[1]);
    ui->plainTextEdit->appendPlainText("物品所在地址：" + attributeList[2]);
    ui->plainTextEdit->appendPlainText("手机号码：" + attributeList[3]);
    ui->plainTextEdit->appendPlainText("邮箱：" + attributeList[4]);
    for (int i = 5; i < attributeList.length(); i++)
        ui->plainTextEdit->appendPlainText(attributeNameList[i - 5] + "：" + attributeList[i]);
}

//鼠标在物品树窗口中点击对象发生改变时的槽函数，在文本框中显示当前点击对象的信息
void MainWindow::on_objectTree_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    Q_UNUSED(previous);     //防止warning
    QStringList list;

    //若点击对象为空，清空文本框
    if (current == NULL) {
        ui->plainTextEdit->clear();
        return;
    }

    //若点击对象为顶层节点，输出其文本内容
    if (current->type() == topItem)
        ui->plainTextEdit->setPlainText(current->text(0));

    //若点击对象为类型节点，输出其类型名称和属性名称
    if (current->type() == typeItem) {
        list = current->data(0, Qt::UserRole).toStringList();
        ui->plainTextEdit->setPlainText("物品类型：");
        ui->plainTextEdit->appendPlainText(current->text(0));
        ui->plainTextEdit->appendPlainText("物品属性：");
        for (int i = 0; i < list.length(); i++)
            ui->plainTextEdit->appendPlainText(list[i]);
    }

    //若点击对象为物品节点，输出其所有信息
    if (current->type() == objectItem) {
        list = current->data(0, Qt::UserRole).toStringList();
        QStringList typeNameList = current->parent()->data(0, Qt::UserRole).toStringList();    //获取物品所属类型的属性名称列表
        ui->plainTextEdit->setPlainText("物品名称：" + list[0]);
        ui->plainTextEdit->appendPlainText("物品说明：" + list[1]);
        ui->plainTextEdit->appendPlainText("物品所在地址：" + list[2]);
        ui->plainTextEdit->appendPlainText("手机号码：" + list[3]);
        ui->plainTextEdit->appendPlainText("邮箱：" + list[4]);
        for (int i = 5; i < list.length(); i++)
            ui->plainTextEdit->appendPlainText(typeNameList[i - 5] + "：" + list[i]);
    }
}

//点击主功能窗口的“删除物品”按钮的槽函数
void MainWindow::on_btnDeleteObject_clicked()
{
    QString typeName = QInputDialog::getItem(this, "选择", "请选择物品类型", typeNameList, 0, false);
    QString objectName = QInputDialog::getText(this, "输入", "请输入要删除的物品名称");
    QString phoneNumber = QInputDialog::getText(this, "输入", "请输入手机号码");
    QTreeWidgetItem* typeItem = searchTypeItem(typeName);
    QTreeWidgetItem* objectItem;

    //遍历该类型节点的所有物品节点
    for (int i = 0; i < typeItem->childCount(); i++) {
        objectItem = typeItem->child(i);
        if (objectName == objectItem->text(0)) {
            if (phoneNumber == objectItem->data(0, Qt::UserRole).toStringList()[3]) {
                typeItem->removeChild(objectItem);
                delete objectItem;
                QMessageBox::information(this, "消息", "物品删除成功");
                return;
            }
        }
    }
    QMessageBox::warning(this, "警告", "未找到物品");
}

void MainWindow::on_btnUser_clicked()
{
    QDlgNewUser dialog;
    dialog.exec();
}

