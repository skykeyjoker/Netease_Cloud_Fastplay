#include "mynetworkcookiejar.h"

MyNetworkCookieJar::MyNetworkCookieJar(QObject *parent) : QNetworkCookieJar(parent)
{

}

MyNetworkCookieJar::~MyNetworkCookieJar()
{

}

QList<QNetworkCookie> MyNetworkCookieJar::getAllCookies()
{
    return allCookies();
}
