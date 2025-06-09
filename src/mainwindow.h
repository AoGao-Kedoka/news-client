#pragma once

#include <QNetworkAccessManager>
#include <QSettings>
#include <QWidget>

#include "news.h"
#include "newscontent.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr, QSettings *settings = nullptr,
                        QNetworkAccessManager *networkManager = nullptr);
    ~MainWindow();

    void showEvent(QShowEvent *event) override;

  private slots:
    void on_FeedFetched();
    void on_ItemsFetched(const std::vector<News::NewsItem> &items);

  private:
    bool validReply(QNetworkReply *reply);

  private:
    std::unique_ptr<Ui::MainWindow> ui;
    News                            news;
    int                             offset = 0;
};