/**
  *  @brief    主函数，新建主窗口
  *  @file     main.cpp
  *  @author   57119203牛佳飞
  *  @version  2.0(版本号)
  *  @date     2021-07-13
  */
#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

