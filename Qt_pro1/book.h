/**
  *  @brief    书籍的描述对象
  *  @file     book.h
  *  @author   57119203牛佳飞
  *  @version  5.0(版本号)
  *  @date     2021-07-20
  */
#ifndef BOOK_H
#define BOOK_H
# include<qstring.h>
#include<QDate>

class Book
{
public:
    Book();
    QString m_strBookName; //书名
    QString m_strISBN; //书目编号，即ISBN号
    QString m_strAuthor; //作者
    QDate m_dtPublicationDate; //出版日期
    QString m_strPublishingHouse; //出版社
    QString m_strRegistrationNumber; //登记号
};
#endif // BOOK_H
