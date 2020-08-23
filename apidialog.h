#ifndef APIDIALOG_H
#define APIDIALOG_H

#include "stable.h"

class APIDialog : public QDialog
{
    Q_OBJECT
public:
    explicit APIDialog(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLay;
    QLabel *info;
    QLineEdit *edit;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;
signals:

};

#endif // APIDIALOG_H
