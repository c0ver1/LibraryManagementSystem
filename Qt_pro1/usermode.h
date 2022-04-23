/**
  *  @brief    用户模式
  *  @file     usermode.h
  *  @author   57119203牛佳飞
  *  @version  7.0(版本号)
  *  @date     2021-07-22
  */

#ifndef USERMODE_H
#define USERMODE_H

#include <QDialog>

namespace Ui {
class UserMode;
}

class UserMode : public QDialog
{
    Q_OBJECT

public:
    explicit UserMode(QString strCurrentUser,QWidget *parent = nullptr);
    QString m_strCurrentUser; //当前用户
    QString m_strSelectRegNum; //当前选中书籍的登记号
    void InitializeTable(); //初始化表格
    ~UserMode();

private slots:
    void on_FindEdit_textChanged(const QString &arg1); //查找，信号为编辑框发生改变
    void on_BorrowButton_clicked(); //借阅
    void on_ReturnButton_clicked(); //还书
    void on_GoBackButton_clicked(); //注销
    void on_FastReturnButton_clicked(); //快速还书
    void on_MyInfo_clicked(); //我的信息

private:
    Ui::UserMode *ui;
};

#endif // USERMODE_H
