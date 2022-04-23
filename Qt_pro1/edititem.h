/**
  *  @brief    管理界面编辑单本书籍
  *  @file     edititem.h
  *  @author   57119203牛佳飞
  *  @version  6.0(版本号)
  *  @date     2021-07-27
  */

#ifndef EDITITEM_H
#define EDITITEM_H

#include <QDialog>

namespace Ui {
class EditItem;
}

class EditItem : public QDialog
{
    Q_OBJECT

public:
    explicit EditItem(QString strCurrentSelectItem,QWidget *parent = nullptr);
    QString m_strSelectedEditItem; //当前选中书的登记号
    void InitializeItem(); //初始化编辑框
    ~EditItem();

private:
    Ui::EditItem *ui;

private slots:
    void on_SaveButton_clicked();  //保存编辑书籍的信息
    void on_BorrowState_currentIndexChanged(int index); //根据借出与否设置借阅日期和借书证号是否可编辑
};

#endif // EDITITEM_H
