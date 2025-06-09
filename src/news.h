#pragma once

#include <QNetworkAccessManager>
#include <QSettings>

/**
 * @brief The News class handles news REST API
 * https://nextcloud.github.io/news/api/api-v1-3/
 */
class News
{
  public:
    News(QSettings *settings, QNetworkAccessManager *networkManager);
    ~News();
    QNetworkReply *FetchFeeds();
    QNetworkReply *FetchItems();

  private:
    QString buildAuthHeader(QString username, QString appPassword);

  public:
    struct NewsItem
    {
        int     id;
        QString title;
        QString url;
        QString body;
        bool    unread;
        QString guid;
    };

    std::vector<NewsItem> NewsItems;

    int DefaultItemCount = 20;

  private:
    QString newsBaseUrl = "/index.php/apps/news/api/v1-3";
    QString username;
    QString appPassword;

    QNetworkAccessManager *networkManager;
    QNetworkRequest        request;
};