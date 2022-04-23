/**
  *  @brief    管理界面创建新条目（书籍）
  *  @file     createitem.cpp
  *  @author   57119203牛佳飞
  *  @version  3.0(版本号)
  *  @date     2021-07-25
  */
#include "createitem.h"
#include "ui_createitem.h"
#include<QMessageBox>
#include<QSqlQuery>
#include<QDebug>

CreateItem::CreateItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateItem)
{
    ui->setupUi(this);
    setWindowTitle("新建"); //设置窗口名称
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint); //取消帮助按钮
    setFixedHeight(350); //设置窗口大小
    setFixedWidth(400);
}

CreateItem::~CreateItem()
{
    delete ui;
}
/* 创建新书籍，当数量为1时，相当于创建单本书籍；不为1时，批量创建 */
void CreateItem::on_SaveButton_clicked()
{
//确保输入的新书籍的各项基本信息不为空
    if(ui->RegNumEdit->text()==""||ui->BookNameEdit->text()==""||ui->AuthorEdit->text()==""|| \
            ui->ISBNEdit->text()==""||ui->PubHouseEdit->text()==""||ui->PubDateEdit->text()=="")
    {
        QMessageBox::information(this, "提示","存在非法输入，创建失败！");
        return;
    }
    else
    {   //获取各项信息
        QString t_strRegNum=ui->RegNumEdit->text();
        QString t_strBookName=ui->BookNameEdit->text();
        QString t_strAuthor=ui->AuthorEdit->text();
        QString t_strISBN=ui->ISBNEdit->text();
        QString t_strPubHouse=ui->PubHouseEdit->text();
        QString t_strPubDate=ui->PubDateEdit->text();
        int t_nInventory=ui->InventoryEdit->value();
        QSqlQuery sqlQuery1,sqlQuery2;
        sqlQuery1.exec(QString("SELECT 登记号 FROM BookInfo WHERE ISBN='%1'").arg(t_strISBN)); //根据ISBN查询登记号
        sqlQuery1.next();
        sqlQuery2.exec(QString("SELECT COUNT(登记号) AS nums FROM BookInfo WHERE ISBN='%1'").arg(t_strISBN)); //根据ISBN查询登记号的数量
        sqlQuery2.next();
        int t_nSize=sqlQuery2.value(0).toInt();
        if(t_nSize==0) //查询结果为0，说明数据库中没有这类书，是“新书”，登记号可以从.1开始
        {
            for(int i=0;i<t_nInventory;++i)
            {
                //向数据库中插入数据，登记号从.1开始递增
                QString t_strCmd=QString("INSERT INTO BookInfo VALUES('%1','%2','%3','%4','%5','%6',1,'','')").arg(t_strRegNum+'.'+QString::number(i+1)) .arg(t_strBookName).arg(t_strAuthor)\
                        .arg(t_strISBN).arg(t_strPubHouse).arg(t_strPubDate);
                QSqlQuery sqlQuery;
                if(! sqlQuery.exec(t_strCmd))
                {
                    QMessageBox::information(this, "提示","创建失败！");
                    break;
                }
                if(i==t_nInventory-1)
                    QMessageBox::information(this, "提示","创建成功！");
            }
        }
        else//数据库中有记录，说明新建的是“旧书”，登记号不能从.1开始自动递增
        {
            QVector<QString> t_vCurrentRegNum;//保存该种书当前已有的登记号
            for(int i=0;i<t_nSize;++i)
            {
                t_vCurrentRegNum.append(sqlQuery1.value(0).toString().section(".",1,1)); //把已有的登记号提取出，以便去重
                sqlQuery1.next();
            }
            int t_nCount=1;//遍历，去和已有登记号比对
            int t_nRealCount=0; //统计可用登记号的个数
            QVector<QString> t_vInsertRegNum;//保存可以新建的登记号
            while(1)
            {
                int t_nFlag=1; //查看该登记号是否已有
                for(int i=0;i<t_vCurrentRegNum.size();++i)
                {
                    if(QString::number(t_nCount)==t_vCurrentRegNum[i])
                    {
                        t_nFlag=0;
                        break;
                    }
                }
                if(t_nFlag==1) //找到了新的登记号
                {
                    t_vInsertRegNum.append(QString::number(t_nCount)); //保存新的可用登记号
                    t_nRealCount++;
                }
                t_nCount++;
                if(t_nRealCount==t_nInventory)
                    break;
            }
            for(int i=0;i<t_nInventory;++i) //利用新的可用登记号，向数据库中插入新记录
            {
                QString t_strCmd=QString("INSERT INTO BookInfo VALUES('%1','%2','%3','%4','%5','%6',1,'','')").arg(t_strRegNum+'.'+t_vInsertRegNum[i]) .arg(t_strBookName).arg(t_strAuthor)\
                        .arg(t_strISBN).arg(t_strPubHouse).arg(t_strPubDate);
                QSqlQuery sqlQuery;
                if(! sqlQuery.exec(t_strCmd))
                {
                    QMessageBox::information(this, "提示","创建失败！");
                    break;
                }
                if(i==t_nInventory-1)
                    QMessageBox::information(this, "提示","创建成功！");
            }

        }
        this->close();
    }
}
