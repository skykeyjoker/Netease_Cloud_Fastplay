#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
    // Window
    setWindowTitle("更换API源");
    setFixedSize(QSize(450,150));
    setWindowFlag(Qt::Dialog);

    // Init UI
    mainLay = new QVBoxLayout(this);
    QTextBrowser *brower = new QTextBrowser;
    mainLay->addWidget(brower);

    brower->setHtml("<p>该项目由Skykey开发，Github项目地址为</p>"
                    "<a href=\"https://github.com/skykeyjoker/Netease_Cloud_Fastplay\">Netease_Cloud_Fastplay</a>"
                    "<p>特别鸣谢：</p>"
                    "<a href=\"https://github.com/ZainCheung/netease-cloud-api\">网易云音乐升级API</a>"
                    "<br>"
                    "<a href=\"https://github.com/bricke/Qt-AES\">AES加密</a>");
    brower->setOpenExternalLinks(true);
}
