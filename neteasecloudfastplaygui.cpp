﻿    #if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "neteasecloudfastplaygui.h"
#include "ui_neteasecloudfastplaygui.h"

NeteaseCloudFastPlayGUI::NeteaseCloudFastPlayGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NeteaseCloudFastPlayGUI)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN32
    setFixedSize(QSize(600,800));
    setWindowIcon(QIcon(":/res/icon.png"));
#endif

#ifdef Q_OS_WIN64
    setFixedSize(QSize(600,800));
    setWindowIcon(QIcon(":/res/icon.png"));
#endif

#ifdef Q_OS_LINUX
    setFixedSize(QSize(600,800));
    setWindowIcon(QIcon(":/res/icon.png"));
#endif

#ifdef Q_OS_MAC
    setFixedSize(QSize(600,800));
    setWindowIcon(QIcon(":/res/icon.png"));
#endif

    // Init UI
    mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);

    mainLay = new QVBoxLayout(mainWidget);

    groupAccount = new QGroupBox("账户");
    groupOperation = new QGroupBox("操作");
    mainLay->addWidget(groupAccount);
    mainLay->addWidget(groupOperation);

    QVBoxLayout *vlay_gruopAccount = new QVBoxLayout(groupAccount);
    QGroupBox *groupAccount_login = new QGroupBox("用户登录");
    QGroupBox *groupAccount_info = new QGroupBox("用户信息");

    vlay_gruopAccount->addWidget(groupAccount_login);
    QGridLayout *glay_login = new QGridLayout(groupAccount_login);
    le_userName = new QLineEdit;
    le_passWord = new QLineEdit;
    radio_email = new QRadioButton("邮箱登录");
    radio_phone = new QRadioButton("手机登录");
    QPushButton *btn_login = new QPushButton("登录");

    glay_login->addWidget(new QLabel("账户名："),0,0);
    glay_login->addWidget(le_userName,0,2);
    glay_login->addWidget(new QLabel("密码："),1,0);
    glay_login->addWidget(le_passWord,1,2);
    glay_login->addWidget(btn_login,0,3,2,1);
    glay_login->addWidget(radio_email,2,0);
    glay_login->addWidget(radio_phone,2,1);

    radio_email->setChecked(true);
    le_passWord->setEchoMode(QLineEdit::Password);

    vlay_gruopAccount->addWidget(groupAccount_info);
    QGridLayout *glay_info = new QGridLayout(groupAccount_info);
    lb_avatar = new QLabel;
    lb_nickName = new QLabel;
    lb_signature = new QLabel;
    lb_level = new QLabel;
    lb_listenSongs = new QLabel;

    glay_info->addWidget(lb_avatar,0,1,1,1);

    glay_info->addWidget(new QLabel("昵称："),3,0);
    glay_info->addWidget(lb_nickName,3,1);

    glay_info->addWidget(new QLabel("个性签名："),4,0);
    glay_info->addWidget(lb_signature,4,1,1,3);

    glay_info->addWidget(new QLabel("等级："),5,0);
    glay_info->addWidget(lb_level,5,1);

    glay_info->addWidget(new QLabel("听歌数："),6,0);
    glay_info->addWidget(lb_listenSongs,6,1);

    // 头像格式
    lb_avatar->setFixedSize(128,128);
    lb_avatar->setScaledContents(true);

    connect(btn_login, &QPushButton::clicked, [=](){
        if(le_userName->text().isEmpty() || le_passWord->text().isEmpty())
        {
            QMessageBox::critical(this,"登陆失败","请填写用户名和密码！");
            return;
        }
        else
        {
            if(radio_email->isChecked())
            {
                login(le_userName->text(),
                      le_passWord->text());
            }
            else
            {
                login(le_userName->text(),
                      le_passWord->text(),
                      false);
            }

        }
    });

    QVBoxLayout *vlay_groupOperation = new QVBoxLayout(groupOperation);
    QGroupBox *groupOperation_Sign = new QGroupBox("签到与打卡");
    QGroupBox *groupOperation_Listen = new QGroupBox("刷歌单");

    vlay_groupOperation->addWidget(groupOperation_Sign);
    QHBoxLayout *hlay_GroupSign = new QHBoxLayout(groupOperation_Sign);
    QPushButton *btn_sign = new QPushButton("签到");
    QPushButton *btn_clock = new QPushButton("打卡");
    hlay_GroupSign->addStretch();
    hlay_GroupSign->addWidget(btn_sign);
    hlay_GroupSign->addStretch();
    hlay_GroupSign->addWidget(btn_clock);
    hlay_GroupSign->addStretch();

    vlay_groupOperation->addWidget(groupOperation_Listen);
    QGridLayout *glay_listen = new QGridLayout(groupOperation_Listen);
    le_playListID = new QLineEdit;
    le_playListTimes = new QLineEdit;
    QPushButton *btn_listen = new QPushButton("听歌");

    glay_listen->addWidget(new QLabel("歌单ID："),0,0);
    glay_listen->addWidget(le_playListID,0,2);

    glay_listen->addWidget(new QLabel("次数："),1,0);
    glay_listen->addWidget(le_playListTimes,1,2);

    glay_listen->addWidget(btn_listen,2,1);

    // 关联签到、打卡按钮信号与槽
    connect(btn_sign, &QPushButton::clicked, this, &NeteaseCloudFastPlayGUI::sign);
    connect(btn_clock, &QPushButton::clicked, this, &NeteaseCloudFastPlayGUI::clockin);

    // 关联听歌信号槽
    connect(btn_listen, &QPushButton::clicked, [=](){
        if(le_playListID->text().isEmpty() || le_playListTimes->text().isEmpty())
        {
            QMessageBox::critical(this, "听歌错误", "请填写歌单ID和次数！");
            return ;
        }
        listenSongs(le_playListID->text(),le_playListTimes->text().toInt());
    });


    // 初始化界面规则
    groupOperation->setEnabled(false);
}

NeteaseCloudFastPlayGUI::~NeteaseCloudFastPlayGUI()
{
    delete ui;
}

void NeteaseCloudFastPlayGUI::getInfos()
{
    QNetworkRequest request;
    request.setUrl(QUrl(tr("http://navi.skykeyjoker.xyz/?do=detail&uid=%1").arg(uid)));

    // 发送 GET 请求
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=](){
        // 如果请求出错
        if(reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::critical(this,"查询失败","用户信息失败！");
            return;
        }

        // 读取返回
        QByteArray buf = reply->readAll();
        qDebug()<<"ref: "<<buf;

        // 将返回数据格式化为 JSON
        QJsonParseError err;
        QJsonDocument docment = QJsonDocument::fromJson(buf, &err);

        // 转化 JSON 失败
        if(err.error != QJsonParseError::NoError)
        {
            qDebug()<<"Can't import json data.";

            QMessageBox::critical(this,"查询失败","用户信息失败！");
            return;
        }
        else // 成功转化 JSON
        {
            // 更新标签信息
            lb_level->setText(QString::number(docment.object().value("level").toInt()));
            lb_listenSongs->setText(QString::number(docment.object().value("listenSongs").toInt()));
            lb_nickName->setText(docment.object().value("profile").toObject().value("nickname").toString());
            lb_signature->setText(docment.object().value("profile").toObject().value("signature").toString());

            // 头像
            avatarUrl = docment.object().value("profile").toObject().value("avatarUrl").toString();
            avatarFileName = avatarUrl.mid(avatarUrl.lastIndexOf("/")+1,avatarUrl.length()-avatarUrl.lastIndexOf("/")-1);
            downloadAvatar();
        }
    });
}

// 下载头像
void NeteaseCloudFastPlayGUI::downloadAvatar()
{
    downloader = new QNetworkAccessManager;

    avatarFile = new QFile(QDir::currentPath()+"/"+avatarFileName);
    if(!avatarFile->open(QFile::WriteOnly))
    {
        QMessageBox::critical(this,"下载头像失败","未能成功下载头像文件！");
        return;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(avatarUrl));
    QNetworkReply *reply = downloader->get(request);

    connect(reply, &QNetworkReply::finished, [=](){
        avatarFile->flush();
        avatarFile->close();

        qDebug()<<"file downloaded";
        qDebug()<<avatarFileName;
        QPixmap pixmap(avatarFileName);
        lb_avatar->setPixmap(pixmap);
    });
    connect(reply, &QNetworkReply::readyRead, [=](){
        if(avatarFile)
            avatarFile->write(reply->readAll());
    });
    connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesRead, qint64 totalBytes){
        qDebug()<<bytesRead<<":"<<totalBytes;
    });
}

// 签到
void NeteaseCloudFastPlayGUI::sign()
{
    // 转化 Cookies
    QVariant var;
    var.setValue(allCookies);

    QNetworkRequest request;
    request.setUrl(QUrl("http://navi.skykeyjoker.xyz/?do=sign"));
    request.setHeader(QNetworkRequest::CookieHeader,var);

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this,[=](){
        if(reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::critical(this,"签到失败","签到失败！");
            return;
        }

        // 读取返回
        QByteArray buf = reply->readAll();

        // 转化 JSON
        QJsonParseError err;
        QJsonDocument document = QJsonDocument::fromJson(buf, &err);

        if(err.error != QJsonParseError::NoError)
        {
            QMessageBox::critical(this, "签到失败", "签到失败，返回值不合法！");
            return;
        }

        // 返回值
        int code = document.object().value("code").toInt();
        if(code != 200)
        {
            QString errStr = document.object().value("msg").toString();
            QMessageBox::critical(this, "签到失败", "签到失败！\n"+errStr);
            return;
        }

        int point = document.object().value("point").toInt();
        QMessageBox::information(this, "签到成功", tr("签到成功，获取%1点！").arg(point));
    });
}

// 打卡
void NeteaseCloudFastPlayGUI::clockin()
{
    shot = 0;

    QMessageBox::information(this, "打卡提醒", "因网易云API限制，打卡需要分三次进行，每次间隔30s，请耐心等待！");

    // 初始化 QTimer
    timer = new QTimer;


    timer->singleShot(0,this,&NeteaseCloudFastPlayGUI::timerOneShot);
    timer->singleShot(30000,this,&NeteaseCloudFastPlayGUI::timerOneShot);
    timer->singleShot(30000,this,&NeteaseCloudFastPlayGUI::timerOneShot);
}

// 听歌
void NeteaseCloudFastPlayGUI::listenSongs(const QString &id, const int &times)
{
    // /?do=listen&id=5101628912&time=300

    // 转化 Cookies
    QVariant var;
    var.setValue(allCookies);

    QNetworkRequest request;
    request.setUrl(QUrl(tr("http://navi.skykeyjoker.xyz/?do=listen&id=%1&time=%2").arg(id).arg(times)));
    request.setHeader(QNetworkRequest::CookieHeader,var);

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this,[=](){
        if(reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::critical(this,"刷单失败","刷单失败！");
            return;
        }

        // 读取返回
        QByteArray buf = reply->readAll();

        // 转化 JSON
        QJsonParseError err;
        QJsonDocument document = QJsonDocument::fromJson(buf, &err);

        if(err.error != QJsonParseError::NoError)
        {
            QMessageBox::critical(this, "刷单失败", "刷单失败，返回值不合法！");
            return;
        }

        // 返回值
        int code = document.object().value("code").toInt();
        if(code != 200)
        {
            QMessageBox::critical(this, "刷单失败", "刷单失败！\n");
            return;
        }

        int point = document.object().value("count").toInt();
        QMessageBox::information(this, "刷单成功", tr("刷单成功，共刷%1首歌！").arg(point));
    });
}


// 登录用户
void NeteaseCloudFastPlayGUI::login(const QString &uin, const QString &password, bool loginFromEmail)
{
    manager = new QNetworkAccessManager;

    // 将密码转换为md5
    QString md5;
    md5 = QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Md5).toHex();
    //qDebug()<<password<<" "<<md5;

    if(loginFromEmail) //邮箱登录
    {
        QNetworkRequest request;
        request.setUrl(QUrl(tr("http://navi.skykeyjoker.xyz/?do=email&uin=%1&pwd=%2").arg(uin).arg(md5)));

        //qDebug()<<request.url();

        // 发送 GET 请求
        QNetworkReply *reply = manager->get(request);

        connect(reply,&QNetworkReply::finished,this,[=](){
            // 如果请求出错
            if(reply->error() != QNetworkReply::NoError)
            {
                QMessageBox::critical(this,"登录失败","登陆失败，请检查用户名和密码，以及检查网络是否畅通！");
                return;
            }

            // 读取返回
            QByteArray buf = reply->readAll();
            //qDebug()<<"err: "<<reply->error();
            //qDebug()<<"ref: "<<buf;

            // 将返回数据格式化为 JSON
            QJsonParseError err;
            QJsonDocument docment = QJsonDocument::fromJson(buf, &err);

            // 转化 JSON 失败
            if(err.error != QJsonParseError::NoError)
            {
                qDebug()<<"Can't import json data.";

                QMessageBox::critical(this,"登录失败","登陆失败，请检查用户名和密码，以及检查网络是否畅通！");
                return;
            }
            else // 成功转化 JSON
            {
                // 获取 HTTP 返回值
                int code = docment.object().value("code").toInt();
                if(code != 200)
                {
                    qDebug()<<"Code Err";
                    qDebug()<<code;
                    QMessageBox::critical(this,"登录失败","登陆失败，请检查用户名和密码，以及检查网络是否畅通！");
                    return;
                }


                // 获取 UID
                QJsonValue accountValue = docment.object().value("account");
                QJsonObject accountObj = accountValue.toObject();

                uid = accountObj.value("id").toInt();

                qDebug()<<"uid: "<<uid;

                // 获取用户信息
                getInfos();
                groupOperation->setEnabled(true);
            }


            // 获取Cookies
            cookieJar = manager->cookieJar();
            allCookies = manager->cookieJar()->cookiesForUrl(QUrl("http://navi.skykeyjoker.xyz"));
            qDebug()<<allCookies;
        });


    }
    else //手机号登录
    {
        QNetworkRequest request;
        request.setUrl(QUrl(tr("http://navi.skykeyjoker.xyz/?do=login&uin=%1&pwd=%2").arg(uin).arg(md5)));

        //qDebug()<<request.url();

        QNetworkReply *reply = manager->get(request);

        connect(reply,&QNetworkReply::finished,this,[=](){

            if(reply->error() != QNetworkReply::NoError)
            {
                QMessageBox::critical(this,"登录失败","登陆失败，请检查用户名和密码，以及检查网络是否畅通！");
                return;
            }

            QByteArray buf = reply->readAll();
            //qDebug()<<"err: "<<reply->error();
            //qDebug()<<"ref: "<<buf;
            QJsonParseError err;
            QJsonDocument docment = QJsonDocument::fromJson(buf, &err);

            if(err.error != QJsonParseError::NoError)
            {
                qDebug()<<"Can't import json data.";

                QMessageBox::critical(this,"登录失败","登陆失败，请检查用户名和密码，以及检查网络是否畅通！");
                return;
            }

            int code = docment.object().value("code").toInt();
            if(code != 200)
            {
                qDebug()<<"Code Err";
                qDebug()<<code;
                QMessageBox::critical(this,"登录失败","登陆失败，请检查用户名和密码，以及检查网络是否畅通！");
                return;
            }
            else
            {
                QJsonValue accountValue = docment.object().value("account");
                QJsonObject accountObj = accountValue.toObject();

                uid = accountObj.value("id").toInt();

                qDebug()<<"uid: "<<uid;

                // 获取用户信息
                getInfos();

                groupOperation->setEnabled(true);
            }


            cookieJar = manager->cookieJar();
            allCookies = manager->cookieJar()->cookiesForUrl(QUrl("http://navi.skykeyjoker.xyz"));
            qDebug()<<allCookies;
        });
    }
}

void NeteaseCloudFastPlayGUI::timerOneShot()
{
    shot++;
    qDebug()<<"Timer Oneshot "<<shot;

    if(shot >= 3)
    {
        QMessageBox::information(this, "打卡成功","打卡成功");
        timer->stop();

        // 重新获取信息
        getInfos();

        return;
    }

    // 转化 Cookies
    QVariant var;
    var.setValue(allCookies);

    QNetworkRequest request;
    request.setUrl(QUrl("http://navi.skykeyjoker.xyz/?do=daka"));
    request.setHeader(QNetworkRequest::CookieHeader,var);

    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this,[=](){
        if(reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::critical(this,"打卡失败","打卡失败！");
            return;
        }

        // 读取返回
        QByteArray buf = reply->readAll();

        // 转化 JSON
        QJsonParseError err;
        QJsonDocument document = QJsonDocument::fromJson(buf, &err);

        if(err.error != QJsonParseError::NoError)
        {
            QMessageBox::critical(this, "打卡失败", "打卡失败，返回值不合法！");
            return;
        }

        // 返回值
        int code = document.object().value("code").toInt();
        if(code != 200)
        {
            QMessageBox::critical(this, "打卡失败", "打卡失败！\n");
            return;
        }
    });
}
