#include "news.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

News::News(QSettings *settings, QNetworkAccessManager *networkManager) : networkManager(networkManager)
{
    username    = settings->value("userName").toString();
    appPassword = settings->value("appPassword").toString();
    newsBaseUrl = settings->value("server").toString() + newsBaseUrl;
}

News::~News()
{}

QNetworkReply *News::FetchFeeds()
{
    QNetworkRequest request(newsBaseUrl + "/feeds");
    QString         authHeader = buildAuthHeader(username, appPassword);

    request.setRawHeader("Authorization", authHeader.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->get(request);
    return reply;
}

QNetworkReply *News::FetchItems()
{
    QUrl      url = newsBaseUrl + "/items";
    QUrlQuery query;
    query.addQueryItem("batchSize", QString::number(DefaultItemCount));
    url.setQuery(query);

    QNetworkRequest request(url);
    QString         authHeader = buildAuthHeader(username, appPassword);

    request.setRawHeader("Authorization", authHeader.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->get(request);
    return reply;
}

QString News::buildAuthHeader(QString username, QString appPassword)
{
    QString    credentials       = username + ":" + appPassword;
    QByteArray base64Credentials = credentials.toUtf8().toBase64();
    return "Basic " + base64Credentials;
}