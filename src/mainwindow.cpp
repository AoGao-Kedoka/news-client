#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QPushButton>
#include <QScrollBar>

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
    news.FetchItems();
    QObject::connect(&news, &News::ItemsFetched, this, &MainWindow::on_ItemsFetched);
    QScrollBar *vScrollBar = ui->newsScrollArea->verticalScrollBar();
    QObject::connect(vScrollBar, &QScrollBar::valueChanged, this, [this](int value) {
        QScrollBar *vScrollBar = ui->newsScrollArea->verticalScrollBar();
        if (value >= vScrollBar->maximum())
        {
            news.FetchItems(20, news.NewsItems[news.NewsItems.size() - 1].id, 3, 0, true);
        }
    });
}

MainWindow::~MainWindow()
{}

void MainWindow::on_FeedFetched()
{}

void MainWindow::on_ItemsFetched(std::vector<News::NewsItem> &items)
{
    for (auto &item : items)
    {
        QString      buttonText = QString("[%1] %2\n").arg(item.unread ? "🔵" : "⚪").arg(item.title);
        QPushButton *button     = new QPushButton(buttonText);
        button->setMinimumHeight(150);
        button->setCheckable(false);
        button->setToolTip(item.url);

        connect(button, &QPushButton::clicked, this, [&]() {
            qDebug() << "Content: " << item.body;
            item.unread = false;
            button->setText(QString("[%1] %2\n").arg("⚪").arg(item.title));
            news.MarkItemAsRead(item);
            auto newsContent = std::make_unique<NewsContent>(nullptr, &item);
            newsContent->setWindowTitle(item.title);
            newsContent->show();
            newsContent->raise();
            newsContent->activateWindow();
            auto _ = newsContent.release();
        });

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