/**
  *  @brief    注册新用户（普通用户）
  *  @file     register.cpp
  *  @author   57119203牛佳飞
  *  @version  3.0(版本号)
  *  @date     2021-07-16
  */

#include "register.h"
#include "ui_Register.h"
#include<QSqlTableModel>
#include<QMessageBox>
#include<QDebug>
#include<QSqlQuery>
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    setWindowTitle("注册"); //设置窗口名称
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint); //取消右上角的帮助按钮
    setFixedHeight(200); //设置窗口大小
    setFixedWidth(300);
}

Register::~Register()
{
    delete ui;
}
/* 注册 */
void Register::on_RegisterButton_clicked()
{
    QString t_strUsername=ui->UsernameEdit->text(); //获取用户名
    if(t_strUsername=="") //用户名为空，报错
    {
        QMessageBox::information(this, "注册提示","请输入正确格式的用户名和密码！");
        return ;
    }
    QString t_strPasswd;
    if(ui->PasswdEdit_1->text()==ui->PasswdEdit_2->text()&&ui->PasswdEdit_1->text()!="") //当两次密码输入相同且不为空时，允许注册
        t_strPasswd=ui->PasswdEdit_1->text();
    else
    {
        QMessageBox::information(this, "注册提示","两次输入密码不一致，请重新输入！");
        ui->PasswdEdit_1->clear();
        ui->PasswdEdit_2->clear();
        return;
    }
    QSqlTableModel *t_model=new QSqlTableModel;
    qDebug()<<t_strUsername;
    t_model->setTable("UserInfo");
    t_model->setFilter(QString("Username='%1'").arg(t_strUsername)); //查找是否已存在该用户名
    t_model->select();
    if(t_model->rowCount()> 0)
    {
        QMessageBox::information(this, "注册提示","用户已存在");
    }
    else //如果不存在该用户名，则允许进行注册
    {
        QString t_strCmd=QString("insert into UserInfo values('%1','%2',';',';',';')").arg(t_strUsername) .arg(t_strPasswd); //将用户名和密码添加到数据库中
        QSqlQuery t_query;
        if(t_query.exec(t_strCmd))
            QMessageBox::information(this, "注册提示","注册成功");
        else
            QMessageBox::information(this, "注册提示","注册失败");
        this->close(); //关闭注册窗口
    }
}
