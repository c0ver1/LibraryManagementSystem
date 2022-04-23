/**
  *  @brief    用户模式
  *  @file     usermode.cpp
  *  @author   57119203牛佳飞
  *  @version  7.0(版本号)
  *  @date     2021-07-22
  */
#include "usermode.h"
#include "ui_usermode.h"
#include"mainwindow.h"
#include"QStandardItemModel"
#include<QDebug>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QSqlTableModel>
#include<QMessageBox>
#include"bookinfo.h"
#include"userinfo.h"

extern QVector<BookInfo> g_vBookInfo;
UserMode::UserMode(QString strCurrentUser,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserMode)
{

    ui->setupUi(this);
    setWindowTitle("图书借阅"); //设置窗口名称
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint); //取消帮助按钮
    InitializeTable(); //初始化表格
    m_strCurrentUser=strCurrentUser; //获取当前用户
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime()); //设置日期时间
    ui->dateTimeEdit->setStyleSheet("background-color:transparent;"); //设置日期时间控件透明

}

UserMode::~UserMode()
{
    delete ui;
}
/* 初始化表格视图 */
void UserMode::InitializeTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(6); //设置列数
    model->setHeaderData(0,Qt::Horizontal,"书名"); //设置列头
    model->setHeaderData(1,Qt::Horizontal,"作者");
    model->setHeaderData(2,Qt::Horizontal,"可借复本");
    model->setHeaderData(3,Qt::Horizontal,"出版社");
    model->setHeaderData(4,Qt::Horizontal,"出版日期");
    model->setHeaderData(5,Qt::Horizontal,"ISBN");
    for(int i=0;i<g_vBookInfo.size();++i)
    {
        model->setItem(i,0,new QStandardItem(g_vBookInfo[i].m_BookInfo[0].m_Book.m_strBookName)); //插入数据
        model->setItem(i,1,new QStandardItem(g_vBookInfo[i].m_BookInfo[0].m_Book.m_strAuthor));
        model->setItem(i,2,new QStandardItem(QString::number(g_vBookInfo[i].m_nRealInventory)));
        model->setItem(i,3,new QStandardItem(g_vBookInfo[i].m_BookInfo[0].m_Book.m_strPublishingHouse));
        model->setItem(i,4,new QStandardItem(g_vBookInfo[i].m_BookInfo[0].m_Book.m_dtPublicationDate.toString("yyyy.MM.dd")));
        model->setItem(i,5,new QStandardItem(g_vBookInfo[i].m_BookInfo[0].m_Book.m_strISBN));
        model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0))); //设置第0列红色字体
        model->item(i,1)->setTextAlignment(Qt::AlignCenter); //设置居中样式
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model->item(i,4)->setTextAlignment(Qt::AlignCenter);
        model->item(i,5)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tableView->setModel(model);//为表格应用model
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter); //设置表头居中
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//所有列都扩展自适应宽度，填充充满整个屏幕宽度
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);//对第0列单独设置固定宽度
    ui->tableView->setColumnWidth(0, 180);//设置固定宽度
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格只读
}

/* 用户界面查找 */
void UserMode::on_FindEdit_textChanged(const QString &arg1)
{

    if(ui->FindEdit->text()=="") //查找框为空时，不进行对列的隐藏
    {
        for(int i=0;i<ui->tableView->model()->rowCount();i++)
            ui->tableView->setRowHidden(i,false);
    }
    else
    {
        QString str=ui->FindEdit->text();  //获取查找框内容
        str.remove(QRegExp("\\s")); //去除空格
        for(int i=0;i<ui->tableView->model()->rowCount();i++) //遍历表格每一行
        {
            ui->tableView->setRowHidden(i,true); //隐藏该列
            QString t_strTmp=""; //临时字符串,保存每一格的内容
            //提取信息
            QAbstractItemModel *model=ui->tableView->model();
            QModelIndex index;
            for(int j=0;j<ui->tableView->model()->columnCount();j++) //遍历该行的每一列
            {
                index=model->index(i,j);
                t_strTmp+=model->data(index).toString();
            }
            t_strTmp.remove(QRegExp("\\s")); //去除每一格内容中的空格
            if(t_strTmp.contains(str,Qt::CaseSensitive)) //如果t_strTmp包含要查找的字符串，则查找成功
                ui->tableView->setRowHidden(i,false);//取消隐藏该列
        }
    }
}
/* 用户界面借书 */
void UserMode::on_BorrowButton_clicked()
{

    int t_nSelectedRow= ui->tableView->currentIndex().row(); //获取当前选中行的下标
    QAbstractItemModel *model = ui->tableView->model ();
    QModelIndex index = model->index(t_nSelectedRow,5);//获取选中行的索引
    QVariant data = model->data(index);//选中行第一列的内容
    QString t_strSelectedItem=data.toString();
    QSqlTableModel *t_model=new QSqlTableModel;
    t_model->setTable("BookInfo");
    t_model->setFilter(QString("ISBN='%1' and 借阅状态='1' ").arg(t_strSelectedItem)); //根据ISBN号和借阅状态查询数据库
    t_model->select();
    if( t_model->rowCount()>0) //查找结果大于0，说明有此书且可借
    {
        QSqlRecord record = t_model->record(0); //获取数据库中的该条记录
        record.setValue("借阅状态", 0); //重设借阅状态
        record.setValue("借阅日期",QDate::currentDate().toString("yyyy-MM-dd").replace(QRegExp("-"), "")); //设置借阅日期
        record.setValue("借书证号",m_strCurrentUser); //设置借书证号
        m_strSelectRegNum=record.value("登记号").toString(); //获取该书的登记号
        t_model->setRecord(0, record);
        t_model->submitAll(); //更新数据库
        QSqlQuery sqlQuery;
        //更新数据库中该用户的借书历史
        sqlQuery.exec(QString("UPDATE UserInfo SET BookHistory=BookHistory||'%1' WHERE Username='%2'").arg(m_strSelectRegNum+";").arg(m_strCurrentUser));
        //更新数据库中该用户的借阅时间
        sqlQuery.exec(QString("UPDATE UserInfo SET BorrowTime=BorrowTime||'%1' WHERE Username='%2'").arg(QDate::currentDate().toString("yyyy.MM.dd")+" "+QTime::currentTime().toString("hh:mm:ss")+";").arg(m_strCurrentUser));
        MainWindow::InitializeBookInfo(); //更新全局书籍信息
        InitializeTable(); //重新初始化当前表格视图
        QMessageBox::information(this, "借阅提示","借阅成功，书籍登记号："+m_strSelectRegNum);
    }
    else
    {
        QMessageBox::information(this, "借阅提示","此书已被借完！");
    }
}

/* 还书 */
void UserMode::on_ReturnButton_clicked()
{
    int t_nSelectedRow= ui->tableView->currentIndex().row();//获取当前选中行的下标
    QAbstractItemModel *model = ui->tableView->model ();
    QModelIndex index = model->index(t_nSelectedRow,5);//获取选中行的索引
    QVariant data = model->data(index); //选中行第一列的内容
    QString t_strSelectedItem=data.toString();
    QSqlTableModel *t_model=new QSqlTableModel;
    t_model->setTable("BookInfo");
    t_model->setFilter(QString("ISBN='%1' and 借书证号='%2' ").arg(t_strSelectedItem).arg(m_strCurrentUser));  //根据ISBN号和借阅状态查询数据库
    t_model->select();
    qDebug()<<t_model->rowCount();
    if( t_model->rowCount()>0) //查找结果大于0，说明的确有此书被借出
    {
        QSqlRecord record = t_model->record(0);//获取数据库中的该条记录
        record.setValue("借阅状态", 1);  //重设借阅状态为可借
        record.setValue("借阅日期","");  //清空借阅日期
        record.setValue("借书证号","");  //清空借书证号
        t_model->setRecord(0, record);
        t_model->submitAll(); //更新数据库
        MainWindow::InitializeBookInfo(); //更新全局书籍信息
        InitializeTable(); //重新初始化当前表格视图
        QMessageBox::information(this, "借阅提示","归还成功！");

    }
    else
    {
        QMessageBox::information(this, "借阅提示","未查询到借阅记录，还书失败！");
    }
}
/* 注销，返回登录界面 */
void UserMode::on_GoBackButton_clicked()
{
    MainWindow* t_MainWindow=new MainWindow;
    this->hide();
    t_MainWindow->show();
}
/* 快速还书 */
void UserMode::on_FastReturnButton_clicked()
{
    QString t_strRegistrationNumber=ui->ReturnBookEdit->text(); //获取文本框中的登记号
    if(t_strRegistrationNumber=="") //确认是否为空
    {
        QMessageBox::information(this, "借阅提示","请输入登记号！");
    }
    QSqlTableModel *t_model=new QSqlTableModel;
    t_model->setTable("BookInfo"); //根据登记号和借书证号查询数据库
    t_model->setFilter(QString("登记号='%1' and 借书证号='%2' ").arg(t_strRegistrationNumber).arg(m_strCurrentUser));
    t_model->select();
    if( t_model->rowCount()>0) //查找成功
    {
        QSqlRecord record = t_model->record(0);
        record.setValue("借阅状态", 1); //重设借阅状态为可借
        record.setValue("借阅日期",""); //清空借阅日期
        record.setValue("借书证号",""); //清空借书证号
        t_model->setRecord(0, record);
        t_model->submitAll(); //更新数据库
        MainWindow::InitializeBookInfo(); //更新全局书籍信息
        InitializeTable(); //重新初始化当前表格视图
        QMessageBox::information(this, "借阅提示","归还成功！");
    }
    else
    {
        QMessageBox::information(this, "借阅提示","未查询到借阅记录，还书失败！");
    }
}

void UserMode::on_MyInfo_clicked() //打开用户个人信息界面
{
    UserInfo t_UserInfoWindow(m_strCurrentUser,m_strSelectRegNum);
    t_UserInfoWindow.exec();

}
