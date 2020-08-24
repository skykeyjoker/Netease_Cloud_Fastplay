#ifndef MYNETWORKCOOKIEJAR_H
#define MYNETWORKCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>

class MyNetworkCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    MyNetworkCookieJar(QObject *parent);
    ~MyNetworkCookieJar();

public:
    QList<QNetworkCookie> getAllCookies();
};

#endif // MYNETWORKCOOKIEJAR_H
