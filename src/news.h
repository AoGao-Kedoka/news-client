#pragma once

#include <QNetworkAccessManager>
#include <QSettings>

/**
 * @brief The News class handles news REST API
 * https://nextcloud.github.io/news/api/api-v1-3/
 */
class News : public QObject
{
    Q_OBJECT

  public:
    News(QSettings *settings, QNetworkAccessManager *networkManager);
    ~News();
    void FetchFeeds();
    void FetchItems(int batchSize = 20, int offset = 0, int type = 3, int id = 0, bool getRead = true);

    struct NewsItem
    {
        int     id;
        QString title;
        QString url;
        QString body;
        bool    unread;
        QString guid;
    };

    void MarkItemAsRead(NewsItem &item);

  private slots:
    void on_FeedsNetworkReplyed();
    void on_ItemsNetworkReplyed();

  private:
    QString buildAuthHeader(QString username, QString appPassword);
    bool    validReply(QNetworkReply *reply);

  public:
    std::vector<NewsItem> NewsItems;

  signals:
    void ItemsFetched(std::vector<NewsItem> &items);

  private:
    QString newsBaseUrl = "/index.php/apps/news/api/v1-3";
    QString username;
    QString appPassword;

    QNetworkAccessManager *networkManager;
    QNetworkRequest        request;
};