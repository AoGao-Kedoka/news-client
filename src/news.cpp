#include "news.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
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

void News::FetchFeeds()
{
    QNetworkRequest request(newsBaseUrl + "/feeds");
    QString         authHeader = buildAuthHeader(username, appPassword);

    request.setRawHeader("Authorization", authHeader.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, &News::on_FeedsNetworkReplyed);
}

void News::FetchItems(int batchSize, int offset, int type, int, bool getRead)
{
    QUrl      url = newsBaseUrl + "/items";
    QUrlQuery query;
    query.addQueryItem("batchSize", QString::number(batchSize));
    query.addQueryItem("offset", QString::number(offset));
    url.setQuery(query);

    QNetworkRequest request(url);
    QString         authHeader = buildAuthHeader(username, appPassword);

    request.setRawHeader("Authorization", authHeader.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->get(request);
    QObject::connect(reply, &QNetworkReply::finished, this, &News::on_ItemsNetworkReplyed);
}

void News::on_ItemsNetworkReplyed()
{
    NewsItems.clear();
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!validReply(reply))
    {
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    QJsonObject   rootObj = jsonDoc.object();
    QJsonArray    items   = rootObj["items"].toArray();
    reply->deleteLater();

    for (const QJsonValue &itemVal : items)
    {
        QJsonObject item = itemVal.toObject();

        News::NewsItem newsItem;
        newsItem.id     = item["id"].toInt();
        newsItem.title  = item["title"].toString();
        newsItem.url    = item["url"].toString();
        newsItem.body   = item["body"].toString();
        newsItem.unread = item["unread"].toBool();
        newsItem.guid   = item["guid"].toString();

        NewsItems.push_back(newsItem);
    }
    emit ItemsFetched(NewsItems);
}

void News::on_FeedsNetworkReplyed()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!validReply(reply))
    {
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    qDebug() << "Feeds fetched successfully:" << response;
    reply->deleteLater();
}

QString News::buildAuthHeader(QString username, QString appPassword)
{
    QString    credentials       = username + ":" + appPassword;
    QByteArray base64Credentials = credentials.toUtf8().toBase64();
    return "Basic " + base64Credentials;
}

bool News::validReply(QNetworkReply *reply)
{
    if (!reply)
        return false;

    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Network error :" << reply->errorString();
        return false;
    }
    return true;
}