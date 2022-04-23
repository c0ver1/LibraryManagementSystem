/**
  *  @brief    管理界面编辑单本书籍
  *  @file     edititem.cpp
  *  @author   57119203牛佳飞
  *  @version  6.0(版本号)
  *  @date     2021-07-27
  */

#include "edititem.h"
#include "ui_edititem.h"
#include<QMessageBox>
#include<QSqlQuery>
#include<QDebug>
#include <QTime>


EditItem::EditItem(QString strCurrentSelectItem,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditItem)
{
    ui->setupUi(this);
    setWindowTitle("编辑书籍"); //设置窗口名称
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint); //取消帮助按钮
    setFixedHeight(500); //设置窗口大小
    setFixedWidth(400);
    ui->BorrowState->addItem("已借出"); //初始化下拉菜单
    ui->BorrowState->addItem("可借");
    m_strSelectedEditItem=strCurrentSelectItem; //获取当前选中书籍的登记号
    InitializeItem();
}

EditItem::~EditItem()
{
    delete ui;
}
void EditItem::InitializeItem() //从数据库读取信息，初始化编辑框
{
    QSqlQuery sqlQuery;
    sqlQuery.exec(QString("SELECT * FROM BookInfo WHERE 登记号 = %1").arg(m_strSelectedEditItem));
    if(!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to query table. " ;
    }
    else
    {
        sqlQuery.next();
        qDebug()<<"SQl"<<sqlQuery.value(0).toString();
        ui->RegNumEdit->setText(sqlQuery.value(0).toString());
        ui->BookNameEdit->setText(sqlQuery.value(1).toString());
        ui->AuthorEdit->setText(sqlQuery.value(2).toString());
        ui->ISBNEdit->setText(sqlQuery.value(3).toString());
        ui->PubHouseEdit->setText(sqlQuery.value(4).toString());
        ui->PubDateEdit->setText(sqlQuery.value(5).toString());
        qDebug()<<sqlQuery.value(6).toBool()<<"BOOL";
        ui->BorrowState->setCurrentIndex(sqlQuery.value(6).toBool());
        ui->BorrowDateEdit->setText(sqlQuery.value(7).toString());
        ui->LibraryCard->setText(sqlQuery.value(8).toString());
    }
}
/* 保存编辑后的信息 */
void EditItem::on_SaveButton_clicked()
{
    //确保各项输入非空
    if(ui->RegNumEdit->text()==""||ui->BookNameEdit->text()==""||ui->AuthorEdit->text()==""|| \
            ui->ISBNEdit->text()==""||ui->PubHouseEdit->text()==""||ui->PubDateEdit->text()=="")
    {
        QMessageBox::information(this, "提示","存在非法输入，编辑失败！");
        return;
    }
    else
    {
        //从控件获取各字段的值
        QString t_strRegNum=ui->RegNumEdit->text();
        QString t_strBookName=ui->BookNameEdit->text();
        QString t_strAuthor=ui->AuthorEdit->text();
        QString t_strISBN=ui->ISBNEdit->text();
        QString t_strPubHouse=ui->PubHouseEdit->text();
        QString t_strPubDate=ui->PubDateEdit->text();
        bool t_bBorrowState=ui->BorrowState->currentIndex();
        QString t_strBorrowDate="";
        QString t_strLibraryCard="";

        if(t_bBorrowState==0)//借出状态下再设置借阅日期和借书证号
        {
            t_strBorrowDate=ui->BorrowDateEdit->text();
            t_strLibraryCard=ui->LibraryCard->text();
            QSqlQuery sqlQuery;
            //更新数据库中该书的借阅日期
            sqlQuery.exec(QString("UPDATE UserInfo SET BookHistory=BookHistory||'%1' WHERE Username='%2'").arg(t_strRegNum+";").arg(t_strLibraryCard));
            //更新数据库中该书的借书证号
            sqlQuery.exec(QString("UPDATE UserInfo SET BorrowTime=BorrowTime||'%1' WHERE Username='%2'").arg(QDate::fromString(t_strBorrowDate,"yyyyMMdd").toString("yyyy.MM.dd")+";").arg(t_strLibraryCard));

        }
        //更新数据库中该书的其他字段
        QString t_strCmd=QString("UPDATE BookInfo SET 登记号='%1',书名='%2',作者='%3',ISBN='%4',出版社='%5',出版日期='%6',借阅状态='%7',借阅日期='%8',借书证号='%9' WHERE 登记号= '%10'").arg(t_strRegNum) .arg(t_strBookName).arg(t_strAuthor)\
                .arg(t_strISBN).arg(t_strPubHouse).arg(t_strPubDate).arg(t_bBorrowState).arg(t_strBorrowDate).arg(t_strLibraryCard).arg(m_strSelectedEditItem);
        QSqlQuery sqlQuery;
        if(! sqlQuery.exec(t_strCmd))
        {
            QMessageBox::information(this, "提示","编辑失败！");
        }
        else
        {
            QMessageBox::information(this, "提示","编辑成功！");
        }

    }
    this->close();
}


//根据是否可借，设置借阅日期和借书证号的编辑状态（是否可编辑）
void EditItem::on_BorrowState_currentIndexChanged(int index)
{
    if(ui->BorrowState->currentIndex()==1)
    {
        ui->BorrowDateEdit->setEnabled(false);
        ui->LibraryCard->setEnabled(false);
    }
    else
    {
        ui->BorrowDateEdit->setEnabled(true);
        ui->LibraryCard->setEnabled(true);
    }
}
