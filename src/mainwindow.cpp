#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent, QSettings *settings, QNetworkAccessManager *networkManager) :
    QWidget(parent), ui(std::make_unique<Ui::MainWindow>()), news(settings, networkManager)
{
    ui->setupUi(this);
    this->setWindowTitle("News");
}

void MainWindow::showEvent(QShowEvent *event)
{
    // auto feedsReply = news.FetchFeeds();
    // QObject::connect(feedsReply, &QNetworkReply::finished, this, &MainWindow::on_FeedFetched);
    auto itemsReply = news.FetchItems();
    QObject::connect(itemsReply, &QNetworkReply::finished, this, &MainWindow::on_ItemsFetched);
}

MainWindow::~MainWindow()
{}

void MainWindow::on_FeedFetched()
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

void MainWindow::on_ItemsFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!validReply(reply))
    {
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    QJsonObject   rootObj = jsonDoc.object();
    QJsonArray    items   = rootObj["items"].toArray();

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

        QString      buttonText = QString("[%1] %2\n").arg(newsItem.unread ? "🔵" : "⚪").arg(newsItem.title);
        QPushButton *button     = new QPushButton(buttonText);
        button->setCheckable(false);
        button->setToolTip(newsItem.url);

        news.NewsItems.push_back(newsItem);
        ui->newsLayout->addWidget(button);
    }
}

bool MainWindow::validReply(QNetworkReply *reply)
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