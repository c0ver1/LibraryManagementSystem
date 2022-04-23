#ifndef CREATEBOOK_H
#define CREATEBOOK_H

#include <QDialog>

namespace Ui {
class CreateBook;
}

class CreateBook : public QDialog
{
    Q_OBJECT

public:
    explicit CreateBook(QWidget *parent = nullptr);
    ~CreateBook();

private:
    Ui::CreateBook *ui;
};

#endif // CREATEBOOK_H
