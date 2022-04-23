/**
  *  @brief    管理界面编辑同种所有书籍
  *  @file     editbook.cpp
  *  @author   57119203牛佳飞
  *  @version  5.0(版本号)
  *  @date     2021-07-27
  */
#include "editbook.h"
#include "ui_editbook.h"
#include<QSqlQuery>
#include<QDebug>
#include<QMessageBox>
EditBook::EditBook(QString strCurrentSelectISBN,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBook)
{
    ui->setupUi(this);
    m_strSelectedEditISBN=strCurrentSelectISBN; //获取当前选中书籍的ISBN号
    setWindowTitle("编辑书籍"); //设置窗口名称
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint); //取消帮助按钮
    setFixedHeight(500); //设置窗口大小
    setFixedWidth(400);
    InitializeBook(); //初始化编辑框
}

EditBook::~EditBook()
{
    delete ui;
}
void EditBook::InitializeBook() //从数据库读取信息，初始化编辑框
{
    QSqlQuery sqlQuery;
    qDebug()<<m_strSelectedEditISBN<<"********";
    sqlQuery.exec(QString("SELECT * FROM BookInfo WHERE ISBN = %1").arg(m_strSelectedEditISBN));
    if(!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to query table. " ;
    }
    else
    {
        sqlQuery.next();
        qDebug()<<"SQl"<<sqlQuery.value(0).toString();
        ui->RegNumEdit->setText(sqlQuery.value(0).toString().section('.',0,0));
        ui->BookNameEdit->setText(sqlQuery.value(1).toString());
        ui->AuthorEdit->setText(sqlQuery.value(2).toString());
        ui->ISBNEdit->setText(sqlQuery.value(3).toString());
        ui->PubHouseEdit->setText(sqlQuery.value(4).toString());
        ui->PubDateEdit->setText(sqlQuery.value(5).toString());
    }

}
/* 保存编辑后的信息 */
void EditBook::on_SaveButton_clicked()
{
    //确保书籍各项基本信息输入非空
    if(ui->RegNumEdit->text()==""||ui->BookNameEdit->text()==""||ui->AuthorEdit->text()==""|| \
            ui->ISBNEdit->text()==""||ui->PubHouseEdit->text()==""||ui->PubDateEdit->text()=="")
    {
        QMessageBox::information(this, "提示","存在非法输入，编辑失败！");
        return;
    }
    else
    {
        //获取各字段的值
        QString t_strRegNum=ui->RegNumEdit->text();
        QString t_strBookName=ui->BookNameEdit->text();
        QString t_strAuthor=ui->AuthorEdit->text();
        QString t_strISBN=ui->ISBNEdit->text();
        QString t_strPubHouse=ui->PubHouseEdit->text();
        QString t_strPubDate=ui->PubDateEdit->text();
        QVector<QString> m_vRegNum;//保存编辑前的登记号
        QSqlQuery sqlQuery;
        //根据选中书籍的ISBN号查询登记号
        sqlQuery.exec(QString("SELECT 登记号 FROM BookInfo WHERE ISBN='%1'").arg(m_strSelectedEditISBN));
        while(sqlQuery.next())
        {
            m_vRegNum.append(sqlQuery.value(0).toString()); //获取编辑前的登记号
        }
        for(int j=0;j<m_vRegNum.size();++j)
        {
            //将新的登记号前缀和旧的登记号后缀组合，更新数据库
            sqlQuery.exec(QString("UPDATE BookInfo SET 登记号='%1' WHERE 登记号='%2'").arg(t_strRegNum+'.'+m_vRegNum[j].section('.',1,1))
                          .arg(m_vRegNum[j]));
        }
        //更新书籍的其他字段
        QString t_strCmd=QString("UPDATE BookInfo SET 书名='%1',作者='%2',ISBN='%3',出版社='%4',出版日期='%5' WHERE ISBN= '%6'").arg(t_strBookName).arg(t_strAuthor)\
                .arg(t_strISBN).arg(t_strPubHouse).arg(t_strPubDate).arg(m_strSelectedEditISBN);
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
