#include "login.h"
#include "ui_login.h"
#include <QSqlQuery>
#include<QSqlTableModel>
#include<QMessageBox>
#include<QDebug>
#include"adminmode.h"
#include"usermode.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{

    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    ui->setupUi(this);
    ui->UserModeButton->setChecked(true);
    setWindowTitle("登录");
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    setFixedHeight(200);
    setFixedWidth(300);
    //show();
}

Login::~Login()
{
    delete ui;
}

void Login::on_login_clicked()
{



}

void Login::on_Register_clicked()
{

    QString t_strUsername=ui->UsernameEdit->text();
    QString t_strPasswd=ui->PasswdEdit->text();
    QSqlTableModel *t_model=new QSqlTableModel;
    qDebug()<<t_strUsername;
    t_model->setTable("UserInfo");
    t_model->setFilter(QString("Username='%1'").arg(t_strUsername));
    t_model->select();


    if(t_model->rowCount()> 0)
    {
        QMessageBox::information(this, "注册提示","用户已存在");
    }
   else
    {
        if(t_strUsername==""||t_strPasswd=="")
        {
            QMessageBox::information(this, "注册提示","请输入正确格式的用户名和密码！");

        }
        else
        {
        QString t_strCmd=QString("insert into UserInfo values('%1','%2')")
                .arg(t_strUsername) .arg(t_strPasswd);
        QSqlQuery t_query;
        t_query.exec(t_strCmd);
        QMessageBox::information(this, "注册提示","注册成功");
        }
    }
}
