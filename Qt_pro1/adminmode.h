/**
  *  @brief    管理模式
  *  @file     adminmode.h
  *  @author   57119203牛佳飞
  *  @version  6.0(版本号)
  *  @date     2021-07-26
  */
#ifndef ADMINMODE_H
#define ADMINMODE_H

#include <QDialog>
#include<QStandardItemModel>
#include<QSortFilterProxyModel>
#include<book.h>
#include<QVector>
namespace Ui {
class AdminMode;
}

class AdminMode : public QDialog
{
    Q_OBJECT

public:
    explicit AdminMode(QString strCurrentUser,QWidget *parent = nullptr);
    Book m_SelectBook; //当前选中的书
    QString m_strCurrentUser; //当前用户
    QStandardItemModel* m_Model; //创建树形视图的model
    QSortFilterProxyModel* m_ProxyModel; //代理model，实现过滤查找
    void InitializeTree(); //初始化树形视图
    ~AdminMode();

private slots:
    void on_DeleteButton_clicked(); //删除书籍按钮
    void slotCustomContextMenu(const QPoint &point); //右键菜单
    void slotDeleteItem(); //删除书籍
    void slotCreateItem(); //创建书籍
    void slotEditItem(); //编辑书籍
    void slotCopyBookInfo(); //复制书籍信息
    void slotCopyBookISBN(); //复制书籍ISBN号
    void on_treeView_clicked(const QModelIndex &index); //获取树形视图当前选中项
    void on_GoBackButton_clicked(); //返回登录窗口
    void on_CreateButton_clicked(); //新建书籍按钮
    void on_FindEdit_2_textChanged(const QString &arg1); //查找
private:
    Ui::AdminMode *ui;
};

#endif // ADMINMODE_H
