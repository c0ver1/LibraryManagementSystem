/**
  *  @brief    管理界面创建新条目（书籍）
  *  @file     createitem.h
  *  @author   57119203牛佳飞
  *  @version  3.0(版本号)
  *  @date     2021-07-25
  */
#ifndef CREATEITEM_H
#define CREATEITEM_H

#include <QDialog>

namespace Ui {
class CreateItem;
}

class CreateItem : public QDialog
{
    Q_OBJECT

public:
    explicit CreateItem(QWidget *parent = nullptr);
    ~CreateItem();

private slots:
    void on_SaveButton_clicked();  //保存创建的书籍

private:
    Ui::CreateItem *ui;
};

#endif // CREATEITEM_H
