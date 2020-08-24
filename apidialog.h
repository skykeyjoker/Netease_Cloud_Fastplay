#ifndef APIDIALOG_H
#define APIDIALOG_H

#include "stable.h"

class APIDialog : public QDialog
{
    Q_OBJECT
public:
    explicit APIDialog(const QString &oldUrl, QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLay;
    QLabel *info;
    QLineEdit *edit;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;

    QString newUrl;
    QString oldUrl;
signals:
    void updateAPIUrl(const QString &url);
public slots:
    void checkValid();
};

#endif // APIDIALOG_H
