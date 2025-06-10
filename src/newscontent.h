#pragma once

#include "news.h"
#include <QWidget>

namespace Ui
{
class NewsContent;
}

class NewsContent : public QWidget
{
    Q_OBJECT

  public:
    explicit NewsContent(QWidget *parent = nullptr, News::NewsItem *item = nullptr);
    ~NewsContent();

  private slots:
    void on_FinishButtonClicked();

  private:
    std::unique_ptr<Ui::NewsContent> ui;
    News::NewsItem                  *newsItem;
};