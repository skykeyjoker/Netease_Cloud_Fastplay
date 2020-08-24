#include "apidialog.h"

APIDialog::APIDialog(const QString &_oldUrl, QWidget *parent)
    : QDialog(parent), oldUrl(_oldUrl)
{
    // Window
    setWindowTitle("更换API源");
    setFixedSize(QSize(300,150));
    setWindowFlag(Qt::Dialog);


    // Init UI
    mainLay = new QVBoxLayout(this);
    info = new QLabel;
    edit = new QLineEdit;

    qDebug()<<oldUrl;
    info->setText("当前API地址：\n" + _oldUrl);

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

    // 连接按钮信号与槽
    connect(btn_cancel, &QPushButton::clicked, [=](){
        close();
    });
    connect(btn_ok, &QPushButton::clicked, this, &APIDialog::checkValid);
}

// 验证API接口是否有效
void APIDialog::checkValid()
{
    if(edit->text().isEmpty())
    {
        QMessageBox::critical(this, "错误", "请填写API地址！");
        return;
    }

    newUrl = edit->text();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(newUrl+"/?do=check"));

    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=](){
        // 获取所有返回
        QByteArray buf = reply->readAll();

        // 转化为JSON
        QJsonDocument doc = QJsonDocument::fromJson(buf);

        int code = doc.object().value("code").toInt();
        if(code != 301)
        {
            QMessageBox::critical(this, "错误", "API验证未通过！");
            return;
        }
        else
        {
            emit updateAPIUrl(newUrl);
            close();
        }
    });
}
