#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "stable.h"
#include <QTextBrowser>

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLay;
signals:

};

#endif // ABOUTDIALOG_H
