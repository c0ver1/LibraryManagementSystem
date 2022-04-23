/**
  *  @brief    注册新用户（普通用户）
  *  @file     register.h
  *  @author   57119203牛佳飞
  *  @version  3.0(版本号)
  *  @date     2021-07-16
  */

#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_RegisterButton_clicked(); //注册按钮

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
