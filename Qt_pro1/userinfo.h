/**
  *  @brief    用户信息
  *  @file     userinfo.h
  *  @author   57119203牛佳飞
  *  @version  3.0(版本号)
  *  @date     2021-07-29
  */
#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>

namespace Ui {
class UserInfo;
}

class UserInfo : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfo(QString t_strCurrentUser ,QString t_strSelectRegNum,QWidget *parent = nullptr);
    QString m_strSelectRegNum; //当前选中书籍的登记号
    QString m_strCurrentUser; //当前用户
    void Initialize(); //初始化表格
    ~UserInfo();

private:
    Ui::UserInfo *ui;
};

#endif // USERINFO_H
