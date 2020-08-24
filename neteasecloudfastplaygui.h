#ifndef NETEASECLOUDFASTPLAYGUI_H
#define NETEASECLOUDFASTPLAYGUI_H

#include "stable.h"
#include "qaesencryption.h"
#include "apidialog.h"
#include "aboutdialog.h"

#include "mynetworkcookiejar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class NeteaseCloudFastPlayGUI; }
QT_END_NAMESPACE

class NeteaseCloudFastPlayGUI : public QMainWindow
{
    Q_OBJECT

public:
    NeteaseCloudFastPlayGUI(QWidget *parent = nullptr);
    ~NeteaseCloudFastPlayGUI();


    bool readAccountDataFromFile();
    bool createAccountDataFile();
    bool saveAccountDataToFile(const QString &userName, const QString &pwd);
    void getInfos();
    void downloadAvatar();
    void sign();
    void clockin();
    void listenSongs(const QString &id , const int &times);
public slots:
    void login(const QString &uin, const QString &password, bool loginFromEmail = true);
    void timerOneShot();
    void insertPwd(const QString &userName);
    void updateAPIUrl(const QString &newUrl);
private slots:
    void on_action_API_triggered();

    void on_action_About_triggered();

private:
    Ui::NeteaseCloudFastPlayGUI *ui;

    QGroupBox *groupAccount;
    QGroupBox *groupOperation;

    QWidget *mainWidget;
    QVBoxLayout *mainLay;

    QLineEdit *le_userName;
    QLineEdit *le_passWord;

    QRadioButton *radio_email;
    QRadioButton *radio_phone;
    QCheckBox *ck_savePass;

    QLabel *lb_avatar;
    QLabel *lb_nickName;
    QLabel *lb_signature;
    QLabel *lb_level;
    QLabel *lb_listenSongs;

    QNetworkAccessManager *manager;
    QList<QNetworkCookie> allCookies;
    QNetworkCookieJar *cookieJar;
    unsigned uid;

    QFile *avatarFile;
    QString avatarFileName;
    QString avatarUrl;
    QNetworkAccessManager *downloader;

    QLineEdit *le_playListID;
    QLineEdit *le_playListTimes;

    int shot = 0;
    QTimer *timer;

    const QString key = "Caolin20001223";
    QMap<QString,QString> accountMap;
    QStringList userNameList;
    QCompleter *completer;
    QString apiUrl = "http://navi.skykeyjoker.xyz";
};
#endif // NETEASECLOUDFASTPLAYGUI_H
