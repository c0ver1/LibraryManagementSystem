/**
  *  @brief    管理模式
  *  @file     adminmode.cpp
  *  @author   57119203牛佳飞
  *  @version  6.0(版本号)
  *  @date     2021-07-26
  */
#include "adminmode.h"
#include "ui_adminmode.h"
#include"mainwindow.h"
#include<QDebug>
#include<QStandardItemModel>
#include"bookinfo.h"
#include"createitem.h"
#include"edititem.h"
#include"editbook.h"
#include<QMessageBox>
#include<QSqlTableModel>
#include<QObject>
#include<QSqlQuery>
#include<QMenu>
#include<QClipboard>
extern QVector<BookInfo> g_vBookInfo; //全局变量，保存全局的书籍和管理信息
AdminMode::AdminMode(QString strCurrentUser,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminMode)
{
    ui->setupUi(this);
    setWindowTitle("管理界面"); //设置标题
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint); //隐藏右上角的帮助按钮
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime()); //设置时间
    ui->dateTimeEdit->setStyleSheet("background-color:transparent;"); //设置时间框的样式
    m_strCurrentUser=strCurrentUser; //初始化当前用户
    InitializeTree();
}
AdminMode::~AdminMode()
{
    delete ui;
}
/* 右键菜单实现 */
void AdminMode::slotCustomContextMenu(const QPoint &point)
{
    if(ui->treeView->hasFocus())	//使鼠标在treeView里右键才弹出菜单
    {
        QMenu menu;
        QString qss = "QMenu{color:#E8E8E8;background:#4D4D4D;margin:2px;}\
                QMenu::item{padding:3px 20px 3px 20px;}\
                QMenu::indicator{width:13px;height:13px;}\
                QMenu::item:selected{color:#E8E8E8;border:0px solid #575757;background:#1E90FF;}\
                QMenu::separator{height:1px;background:#757575;}";      //设置样式表
                menu.setStyleSheet(qss);                                         //给菜单设置样式

        menu.addAction( QStringLiteral("编辑"), this, SLOT(slotEditItem()));      //设置菜单
        menu.addAction(QStringLiteral("删除"), this, SLOT(slotDeleteItem()));
        menu.addAction(QStringLiteral("新建"), this, SLOT(slotCreateItem()));

        QAction* actionParent = menu.addAction(QStringLiteral("复制"));    //父菜单
        QMenu* subMenu = new QMenu(&menu);  //子菜单
        subMenu->addAction(QStringLiteral("全部信息"), this, SLOT(slotCopyBookInfo()));
        subMenu->addAction(QStringLiteral("ISBN"), this, SLOT(slotCopyBookISBN()));
        actionParent->setMenu(subMenu); //应用子菜单
        menu.exec(this->mapToGlobal(point));//应用右键菜单
    }
}
/* 删除书籍 */
void AdminMode::slotDeleteItem()
{
    int t_nWarning = QMessageBox::warning(this,tr("删除"),tr("你确定删除当前书籍吗？"),QMessageBox::Yes,QMessageBox::No);
    if(t_nWarning==QMessageBox::No)
        return;
    else
    {
        QString deleteSql="";
        if(m_SelectBook.m_strISBN=="")//如果ISBN为空，说明是二级子结点，选中和删除的是单本书籍
        {
            deleteSql = QString ("DELETE FROM BookInfo WHERE 登记号 = %1").arg(m_SelectBook.m_strRegistrationNumber);
        }
        else//否则，选中的是一级父结点，删除同种所有书籍
        {
            deleteSql = QString ("DELETE FROM BookInfo WHERE ISBN = %1").arg(m_SelectBook.m_strISBN);
        }

        // 执行插入的sql语句
        QSqlQuery query;
        query.exec (deleteSql);
        QMessageBox::information(this, "提示","删除成功");
        MainWindow::InitializeBookInfo();
        InitializeTree(); //重新初始化树形视图
    }
}
void AdminMode::slotCreateItem()  //打开新建窗口
{
    CreateItem t_CreateItemWindow;
    t_CreateItemWindow.exec();
    MainWindow::InitializeBookInfo();
    InitializeTree();

}
void AdminMode::slotEditItem() //打开编辑窗口
{

    if(m_SelectBook.m_strISBN=="") //编辑单本书籍
    {
        EditItem t_EditItemWindow(m_SelectBook.m_strRegistrationNumber);
        t_EditItemWindow.exec();
    }
    else //编辑同种所有书籍
    {
        EditBook t_EditBookWindow(m_SelectBook.m_strISBN);
        t_EditBookWindow.exec();
    }
    MainWindow::InitializeBookInfo(); //重新初始化全局书籍信息
    InitializeTree(); //重新初始化树形视图
}
void AdminMode::slotCopyBookInfo() //复制全部信息
{
    QClipboard *t_pClip = QApplication::clipboard();
    t_pClip->setText(QString("%1\n%2\n%3\n%4\n%5").arg(m_SelectBook.m_strBookName).arg(m_SelectBook.m_strAuthor)
                     .arg(m_SelectBook.m_strISBN).arg(m_SelectBook.m_dtPublicationDate.toString("yyyy-MM-dd"))
                     .arg(m_SelectBook.m_strPublishingHouse));
}
void AdminMode::slotCopyBookISBN() //复制ISBN号
{
    QClipboard *t_pClip = QApplication::clipboard();
    t_pClip->setText(m_SelectBook.m_strISBN);
}
/* 初始化树形视图 */
void AdminMode::InitializeTree()
{
    MainWindow::InitializeBookInfo(); //更新全局书籍信息
    ui->treeView->header()->setDefaultAlignment(Qt::AlignCenter);  //设置居中
    ui->treeView->header()->setStretchLastSection(true);  //设置其他列自适应列宽
    ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);			//单元格不能编辑
    ui->treeView->setSelectionBehavior(QTreeView::SelectRows);			//一次选中整行
    ui->treeView->setAlternatingRowColors(true);                  //每间隔一行颜色不一样，当有qss时该属性无效

    m_Model = new QStandardItemModel(ui->treeView);
    //设置表头
    m_Model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("登记号") << QStringLiteral("书名")<<QStringLiteral("作者")<<QStringLiteral("库存数")<<QStringLiteral("ISBN")
                                       <<QStringLiteral("出版日期")<<QStringLiteral("出版社")<<QStringLiteral("借阅状态")<<QStringLiteral("借阅日期")<<QStringLiteral("借书证号"));     //设置列头
    for(int i=0;i<g_vBookInfo.size();++i) //添加一级结点，即父结点
    {
        QList<QStandardItem*> t_itemsLv1;
        QVector<QString> t_vItem0;  //暂存每一列的信息
        t_vItem0.append(g_vBookInfo[i].m_strRegistrationNumber);
        t_vItem0.append(g_vBookInfo[i].m_BookInfo[0].m_Book.m_strBookName);
        t_vItem0.append(g_vBookInfo[i].m_BookInfo[0].m_Book.m_strAuthor);
        t_vItem0.append(QString::number(g_vBookInfo[i].m_nRealInventory)+"/"+QString::number(g_vBookInfo[i].m_nInventory));
        t_vItem0.append(g_vBookInfo[i].m_BookInfo[0].m_Book.m_strISBN);
        t_vItem0.append(g_vBookInfo[i].m_BookInfo[0].m_Book.m_dtPublicationDate.toString("yyyy.MM.dd"));
        t_vItem0.append(g_vBookInfo[i].m_BookInfo[0].m_Book.m_strPublishingHouse);

        for(int p=0;p<3;++p)
            t_vItem0.append(""); //因为是父结点项，后面的3列（书籍的具体借阅信息）都不需要添加内容
        QStandardItem* t_item0=new QStandardItem(t_vItem0[0]);
        t_itemsLv1.append(t_item0);
        for(int m=1;m<t_vItem0.size();++m)
        {
            QStandardItem* t_item=new QStandardItem(t_vItem0[m]);
            t_itemsLv1.append(t_item);
        }
        t_vItem0.clear(); //清空暂存变量
        m_Model->appendRow(t_itemsLv1); //插入一个完整的一级结点到树形视图中

        for(int j=0;j<g_vBookInfo[i].m_BookInfo.size();++j) //添加二级结点
        {


            QList<QStandardItem*> t_itemsLv2;
            QVector<QString> t_vItem;
            t_vItem.append(g_vBookInfo[i].m_BookInfo[j].m_Book.m_strRegistrationNumber);
            t_vItem.append(g_vBookInfo[i].m_BookInfo[j].m_Book.m_strBookName);
            t_vItem.append(g_vBookInfo[i].m_BookInfo[j].m_Book.m_strAuthor);

            for(int i=0;i<4;i++)  //因为是二级结点，中间的一些列（书籍自身的具体信息，在父结点可以体现）不用填写
                t_vItem.append("");
            if(g_vBookInfo[i].m_BookInfo[j].m_bIsBorrowed==1) //判断每一本书籍的借阅状态
            {
                t_vItem.append("可借");
                t_vItem.append("");
            }
            else
            {
                t_vItem.append("已借出");
                t_vItem.append(g_vBookInfo[i].m_BookInfo[j].m_dtBorrowingDate.toString("yyyy.MM.dd"));
            }
            t_vItem.append(g_vBookInfo[i].m_BookInfo[j].m_strLibraryCardNumber);

            for(int m=0;m<t_vItem.size();++m)
            {
                QStandardItem* t_item=new QStandardItem(t_vItem[m]);
                t_itemsLv2.append(t_item);
            }
            t_vItem.clear(); //清空暂存变量
            t_item0->appendRow(t_itemsLv2); //插入一个完整的二级结点到树形视图中
        }
    }

    ui->treeView->setModel(m_Model); //为树形视图应用上面得到的model
    ui->treeView->setColumnWidth(1,180); //设置第一列宽度（书名那一列）
    setContextMenuPolicy(Qt::CustomContextMenu);                           //设置treeView支持右键弹出菜单
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),this,
            SLOT(slotCustomContextMenu(const QPoint &)));  //连接点击右键信号与槽函数

}
/* 删除书籍 */
void AdminMode::on_DeleteButton_clicked()
{
    int t_nWarning = QMessageBox::warning(this,tr("删除"),tr("你确定删除当前书籍吗？"),QMessageBox::Yes,QMessageBox::No);
    if(t_nWarning==QMessageBox::No)
        return;
    else
    {
        QString deleteSql = QString ("DELETE FROM BookInfo WHERE 登记号 = %1").arg(m_SelectBook.m_strRegistrationNumber);
        // 执行插入的sql语句
        QSqlQuery query;
        query.exec (deleteSql);
        QMessageBox::information(this, "提示","删除成功");
        MainWindow::InitializeBookInfo();
        InitializeTree();

    }

}
/* 获取当前选中条目的信息 */
void AdminMode::on_treeView_clicked(const QModelIndex &index)
{
    m_SelectBook.m_strRegistrationNumber=index.sibling(index.row(),0).data().toString(); //获取该行第0列的内容
    m_SelectBook.m_strBookName=index.sibling(index.row(),1).data().toString(); //获取该行第1列的内容，下面类似
    m_SelectBook.m_strAuthor=index.sibling(index.row(),2).data().toString();
    m_SelectBook.m_strISBN=index.sibling(index.row(),4).data().toString();
    m_SelectBook.m_dtPublicationDate=index.sibling(index.row(),5).data().toDate();
    m_SelectBook.m_strPublishingHouse=index.sibling(index.row(),6).data().toString();
}
/* 注销，返回登录界面 */
void AdminMode::on_GoBackButton_clicked() //注销，返回登录界面
{
    MainWindow* t_MainWindow=new MainWindow;
    this->hide();
    t_MainWindow->show();
}
/* 新建按钮 */
void AdminMode::on_CreateButton_clicked() //新建按钮
{
    CreateItem t_CreateItemWindow;
    t_CreateItemWindow.exec();
    InitializeTree();
}
/* 管理员界面查找 */
void AdminMode::on_FindEdit_2_textChanged(const QString &arg1)
{
    QString t_strFind=ui->FindEdit_2->text(); //获取查找文本框的内容

    m_ProxyModel = new QSortFilterProxyModel; //设置代理model
    m_ProxyModel->setSourceModel(m_Model); //设置源model
    m_ProxyModel->setFilterKeyColumn(-1);//设置过滤列为所有列
    ui->treeView->setModel(m_ProxyModel);//应用到树形视图
    QRegExp regExp(t_strFind, Qt::CaseInsensitive, QRegExp::FixedString);
    m_ProxyModel->setFilterRegExp(regExp);//对关键词进行查找过滤
}
