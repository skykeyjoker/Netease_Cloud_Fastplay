#include "apidialog.h"

APIDialog::APIDialog(QWidget *parent) : QDialog(parent)
{
    // Window
    setWindowTitle("更换API源");
    setFixedSize(QSize(300,100));

    // Init UI
    mainLay = new QVBoxLayout(this);
    info = new QLabel;
    edit = new QLineEdit;

    QHBoxLayout *hlay_btns = new QHBoxLayout;
    btn_ok = new QPushButton("确认");
    btn_cancel = new QPushButton("取消");
    hlay_btns->addStretch();
    hlay_btns->addWidget(btn_cancel);
    hlay_btns->addStretch();
    hlay_btns->addWidget(btn_ok);
    hlay_btns->addStretch();

    mainLay->addWidget(info);
    mainLay->addWidget(edit);
    mainLay->addLayout(hlay_btns);
}
