/**
  *  @brief    管理界面编辑同种所有书籍
  *  @file     editbook.h
  *  @author   57119203牛佳飞
  *  @version  5.0(版本号)
  *  @date     2021-07-27
  */
#ifndef EDITBOOK_H
#define EDITBOOK_H

#include <QDialog>

namespace Ui {
class EditBook;
}

class EditBook : public QDialog
{
    Q_OBJECT

public:
    explicit EditBook(QString strCurrentSelectItem,QWidget *parent = nullptr);
    QString m_strSelectedEditISBN; //当前选中书的ISBN号
    void InitializeBook(); //初始化编辑框
    ~EditBook();

private slots:
    void on_SaveButton_clicked(); //保存编辑书籍的信息

private:
    Ui::EditBook *ui;
};

#endif // EDITBOOK_H
