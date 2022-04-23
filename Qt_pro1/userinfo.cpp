/**
  *  @brief    用户信息
  *  @file     userinfo.cpp
  *  @author   57119203牛佳飞
  *  @version  3.0(版本号)
  *  @date     2021-07-29
  */
#include "userinfo.h"
#include "ui_userinfo.h"
#include<QSqlQuery>
#include<QDebug>
#include<QStandardItemModel>

UserInfo::UserInfo(QString t_strCurrentUser,QString t_strSelectRegNum,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfo)
{
    ui->setupUi(this);
    setWindowTitle("我的信息"); //设置窗口名称
    m_strCurrentUser=t_strCurrentUser; //获取当前用户名
    m_strSelectRegNum=t_strSelectRegNum; //获取选中书籍的登记号
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint); //取消帮助按钮
    setFixedHeight(350); //设置窗口大小
    setFixedWidth(1000);
    ui->UserName->setText(m_strCurrentUser); //设置用户名
    ui->UserName->setReadOnly(true); //设置用户名只读
    Initialize(); //初始化表格
}

UserInfo::~UserInfo()
{
    delete ui;
}
/* 初始化表格 */
void UserInfo::Initialize() //用数据库初始化表格
{
    QSqlQuery sqlQuery;
    sqlQuery.exec(QString("SELECT LoginHistory FROM UserInfo WHERE Username='%1'").arg(m_strCurrentUser)); //查询用户登录历史
    sqlQuery.next();
    QString t_strLoginTime=sqlQuery.value(0).toString().section(";",-3,-3);//提取上次登录的日期和时间
    ui->LastLoginTime->setText(t_strLoginTime);
    ui->LastLoginTime->setReadOnly(true);
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(5); //设置表格列数为5：
    sqlQuery.exec(QString("SELECT BookHistory FROM UserInfo WHERE Username='%1'").arg(m_strCurrentUser)); //查询书籍借阅历史
    sqlQuery.next();
    QString t_strBookHistory=sqlQuery.value(0).toString();
    sqlQuery.exec(QString("SELECT BorrowTime FROM UserInfo WHERE Username='%1'").arg(m_strCurrentUser)); //查询书籍借阅时间
    sqlQuery.next();
    QString t_strBorrowTime=sqlQuery.value(0).toString();

    QStringList t_strBookHistoryList = t_strBookHistory.split(";"); //根据分号分隔多个借阅记录
    QStringList t_strBorrowTimeList=t_strBorrowTime.split(";");

    model->setHeaderData(0,Qt::Horizontal,"登记号");
    model->setHeaderData(1,Qt::Horizontal,"书名");
    model->setHeaderData(2,Qt::Horizontal,"ISBN");
    model->setHeaderData(3,Qt::Horizontal,"借阅状态");
    model->setHeaderData(4,Qt::Horizontal,"借阅时间");

    int t_nRowCount=t_strBookHistoryList.size()-2; //除去最开始的分号前的空白和最后面的分号之后的空白
    for(int i=0;i<t_nRowCount;++i)
    {
        model->setItem(i,0,new QStandardItem(t_strBookHistoryList[t_nRowCount-i])); //倒序插入，把时间近的插入到靠前的列
        sqlQuery.exec(QString("SELECT *FROM BookInfo WHERE 登记号='%1'").arg(t_strBookHistoryList[t_nRowCount-i]));
        sqlQuery.next();
        model->setItem(i,1,new QStandardItem(sqlQuery.value(1).toString())); //设置表格的内容
        model->setItem(i,2,new QStandardItem(sqlQuery.value(3).toString()));
        if(sqlQuery.value(6).toBool()==1)
            model->setItem(i,3,new QStandardItem("已还"));
        else
            model->setItem(i,3,new QStandardItem("未还"));
        model->setItem(i,4,new QStandardItem(t_strBorrowTimeList[t_nRowCount-i]));
        model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0))); //设置第0列字的颜色
        model->item(i,0)->setTextAlignment(Qt::AlignCenter); //设置对齐
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model->item(i,3)->setTextAlignment(Qt::AlignCenter);

    }
    ui->RecentBorrowTable->setModel(model);//为表格应用model
    ui->RecentBorrowTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);//表头居中对齐
    ui->RecentBorrowTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//所有列都扩展自适应宽度，填充充满整个屏幕宽度
    ui->RecentBorrowTable->setColumnWidth(0, 100);//设置固定宽度
    ui->RecentBorrowTable->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->RecentBorrowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格只读
}
