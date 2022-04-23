/**
  *  @brief    书籍的管理信息对象
  *  @file     bookinfo.h
  *  @author   57119203牛佳飞
  *  @version  5.0(版本号)
  *  @date     2021-07-20
  */
#ifndef BOOKINFO_H
#define BOOKINFO_H
#include"book.h"
#include<QDate>
#include<QVector>
#include<QMap>
typedef struct
{
    Book m_Book; //书籍自身信息
    bool m_bIsBorrowed; //借阅状态
    QDate m_dtBorrowingDate; //借阅日期
    QString m_strLibraryCardNumber; //借书证号
}g_BookInfo;

class BookInfo
{
public:
    QVector<g_BookInfo> m_BookInfo;  //创建一个元素为包含书自身信息和管理信息的向量
    QString m_strRegistrationNumber; //登记号
    int m_nInventory; //总库存
    int m_nRealInventory; //实际库存
    BookInfo();
};


#endif // BOOKINFO_H
