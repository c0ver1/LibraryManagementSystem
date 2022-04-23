/**
  *  @brief    主窗口，初始化全局变量，设置窗口样式
  *  @file     mainwindow.cpp
  *  @author   57119203牛佳飞
  *  @version  3.0(版本号)
  *  @date     2021-07-16
  */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QSqlQuery>
#include<QMessageBox>
#include<QSqlTableModel>
#include<QPainter>
#include<QPaintEvent>
#include<QToolButton>
#include"adminmode.h"
#include"usermode.h"
#include"register.h"
#include<cstdlib>
QVector<BookInfo> g_vBookInfo;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    if(g_vBookInfo.empty()) //当全局书籍信息为空时，再连接数据库初始化，防止新建主窗口时重复初始化
        ConnectDatabase();
    ui->setupUi(this);
    ui->UserModeButton->setChecked(true); //设置默认选中“用户”模式
    setWindowTitle("图书管理系统"); //设置标题
    setWindowFlags(Qt::FramelessWindowHint);//设置无边框样式
    setFixedSize(1160,658); //设置窗口大小
    QToolButton *minButton = new QToolButton(this);
    QToolButton *closeButton= new QToolButton(this);
    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton); //重设最小化按钮
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);//重设关闭按钮
    minButton->setIcon(minPix); //设置最小化按钮图标
    closeButton->setIcon(closePix); //设置关闭按钮图标
    minButton->setGeometry(MainWindow::width()-45,5,20,20); //设置按钮的位置
    closeButton->setGeometry(MainWindow::width()-25,5,20,20);
    minButton->setStyleSheet("background-color:transparent;");
    closeButton->setStyleSheet("background-color:transparent;");
    connect(minButton, SIGNAL(clicked()), this, SLOT(MinWindow())); //连接按钮的信号和槽
    connect(closeButton, SIGNAL(clicked()), this, SLOT(CloseWindow()));

    InitializeBookInfo();
}
void MainWindow::MinWindow() //最小化窗口
{
    this->showMinimized();
}
void MainWindow::CloseWindow() //关闭窗口
{
    this->close();
}
void MainWindow::ConnectDatabase() //连接数据库
{
    QSqlDatabase g_dbAccount=QSqlDatabase::addDatabase("QSQLITE"); //建立和SQlite数据库的连接
    g_dbAccount.setDatabaseName("MyLibrary.db"); //选择数据库
    if(g_dbAccount.open()) //打开数据库
    {
        qDebug()<<"连接成功";
        QMessageBox::about(NULL,"提示","数据库连接成功");
    }
    else
    {
        QMessageBox::about(NULL,"提示","数据库连接失败");
        exit(0);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
/* 实现窗口拖动 */
void MainWindow::mouseMoveEvent(QMouseEvent *event) //获取鼠标左键按下时移动的路径
{
    if (m_bPressed)
        move(event->pos() - m_point + pos());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) //放起鼠标左键
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void MainWindow::mousePressEvent(QMouseEvent *event) //按下鼠标左键
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_point = event->pos();
    }
}

/* 从数据库读取信息，初始化程序的数据结构 */
void MainWindow::InitializeBookInfo()
{

    g_vBookInfo.clear();
    QSqlQuery t_query;
    t_query.prepare("SELECT * FROM BookInfo");  //查询数据库
    t_query.exec();

    int index=0; //记录读取到的信息，用于特殊处理第一条
    BookInfo t_tmp; //暂存读取到的信息
    while(t_query.next())
    {

        QString t_strRegistrationNumber=t_query.value("登记号").toString().section('.',0,0); //登记号小数点前的数字，用来标识同种书籍
        g_BookInfo t_BookInfo; //暂存读取到的信息
        t_BookInfo.m_Book.m_strISBN=t_query.value("ISBN").toString();
        t_BookInfo.m_Book.m_strAuthor=t_query.value("作者").toString();
        t_BookInfo.m_Book.m_strBookName=t_query.value("书名").toString();
        t_BookInfo.m_Book.m_strPublishingHouse=t_query.value("出版社").toString();
        t_BookInfo.m_Book.m_dtPublicationDate=QDate::fromString(t_query.value("出版日期").toString(),"yyyyMMdd");
        t_BookInfo.m_Book.m_strRegistrationNumber=t_query.value("登记号").toString();
        t_BookInfo.m_strLibraryCardNumber=t_query.value("借书证号").toString();
        t_BookInfo.m_bIsBorrowed=t_query.value("借阅状态").toBool();
        t_BookInfo.m_dtBorrowingDate=QDate::fromString(t_query.value("借阅日期").toString(),"yyyyMMdd");
        if(index!=0&&t_strRegistrationNumber!=t_tmp.m_strRegistrationNumber) //添加新的父结点项
        {
            g_vBookInfo.append(t_tmp);
            t_tmp.m_nInventory=0;
            t_tmp.m_nRealInventory=0;
            t_tmp.m_BookInfo.clear();
        }
        if(t_BookInfo.m_bIsBorrowed==1) //可借，将实际库存+1
            t_tmp.m_nRealInventory++;
        t_tmp.m_nInventory++;  //总库存++
        t_tmp.m_BookInfo.append(t_BookInfo); //父结点项添加子结点
        t_tmp.m_strRegistrationNumber=t_strRegistrationNumber; //更新用于判断同种书籍的登记号前缀
        index++;
    }
    g_vBookInfo.append(t_tmp); //上面的循环中由于循环条件会漏掉最后一个结点，这里单独处理
}


/* 登录 */
void MainWindow::on_LoginButton_clicked()
{
    QString t_strUsername=ui->UsernameEdit->text(); //获取用户名
    QString t_strPasswd=ui->PasswdEdit->text(); //获取密码
    if(ui->AdminModeButton->isChecked()) //查看选择的登录模式
    {
        if(t_strUsername=="admin"&&t_strPasswd=="admin") //检查管理员用户名和密码
        {
            QMessageBox::information(this, "登录提示","登录成功，欢迎"+t_strUsername+"!");
            AdminMode t_AdminWindow(t_strUsername); //新建管理系统的窗口，并将当前用户名传给新窗口
            this->hide(); //隐藏主窗口
            t_AdminWindow.exec();
        }
        else
            QMessageBox::information(this, "登录提示","用户名或密码错误，请重新输入！");
    }
    else
    {

        QSqlTableModel *t_model=new QSqlTableModel; //构造model查询数据库
        t_model->setTable("UserInfo");
        t_model->setFilter(QString("Username='%1' and Passwd='%2'")
                           .arg(t_strUsername).arg(t_strPasswd));
        t_model->select();
        if( t_model->rowCount()) //如果行数不为0，则说明查到该记录，用户名和密码匹配且正确
        {
            QSqlQuery sqlQuery;
            sqlQuery.exec(QString("UPDATE UserInfo SET LoginHistory=LoginHistory||'%1' WHERE Username='%2'").arg(QDate::currentDate().toString("yyyy.MM.dd")+" "+QTime::currentTime().toString("hh:mm:ss")+";")
                          .arg(t_strUsername));
            QMessageBox::information(this, "登录提示","登录成功，欢迎"+t_strUsername);
            UserMode t_UserWindow(t_strUsername); //新建用户系统的窗口，并将当前用户名传给新窗口
            this->hide(); //隐藏主窗口
            t_UserWindow.exec();
        }
        else
            QMessageBox::information(this, "登录提示","用户名或密码错误，请重新输入！");
    }
}
void MainWindow::on_RegisterButton_clicked() //新建注册窗口
{
    Register t_Reg_Window;
    t_Reg_Window.exec();
}

void MainWindow::paintEvent(QPaintEvent *event)//设置背景图片
{
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建QPixmap对象
    QPixmap pix;
    //加载图片
    pix.load(":/Resource/background.png");
    //绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

