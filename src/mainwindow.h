#pragma once

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QSettings>

#include "news.h"
#include "newscontent.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr, QSettings *settings = nullptr,
                        QNetworkAccessManager *networkManager = nullptr);
    ~MainWindow();

    void showEvent(QShowEvent *event) override;

  private slots:
    void on_FeedFetched();
    void on_ItemsFetched(std::vector<News::NewsItem> &items);
    void on_allItemsReadButton_clicked();

  private:
    bool validReply(QNetworkReply *reply);

  private:
    std::unique_ptr<Ui::MainWindow> ui;
    News                            news;
    int                             offset = 0;
};