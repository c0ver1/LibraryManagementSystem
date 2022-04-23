/**
  *  @brief    主窗口，初始化全局变量，设置窗口样式
  *  @file     mainwindow.h
  *  @author   57119203牛佳飞
  *  @version  3.0(版本号)
  *  @date     2021-07-16
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMouseEvent>
#include<QStandardItemModel>
#include"bookinfo.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
extern QVector<BookInfo> g_vBookInfo;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event); //设置背景
    static void InitializeBookInfo(); //初始化全局书籍信息的向量
    bool m_bPressed = true;
    QPoint m_point;
    void mouseReleaseEvent(QMouseEvent *event);   //鼠标事件，左键松开
    void mousePressEvent(QMouseEvent *event); //鼠标事件，左键按下
    void mouseMoveEvent(QMouseEvent *event);//鼠标事件，移动


    ~MainWindow();

private slots:
    void MinWindow(); //最小化窗口
    void CloseWindow(); //最大化窗口
    void on_LoginButton_clicked(); //登录
    void on_RegisterButton_clicked(); //注册

private:
    Ui::MainWindow *ui;
    void ConnectDatabase(); //连接数据库
};
#endif // MAINWINDOW_H
