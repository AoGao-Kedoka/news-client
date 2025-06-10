#include "newscontent.h"
#include "ui_newscontent.h"
#include <QDesktopServices>
#include <QVBoxLayout>

NewsContent::NewsContent(QWidget *parent, News::NewsItem *item) :
    QWidget(parent), ui(std::make_unique<Ui::NewsContent>()), newsItem(item)
{
    ui->setupUi(this);

    if (newsItem)
    {
        ui->newsContentBrowser->setHtml(newsItem->body);
        ui->urlLabel->setText(QString("<a href=\"%1\">%1</a>").arg(newsItem->url));
        ui->urlLabel->setTextFormat(Qt::RichText);
        ui->urlLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        ui->urlLabel->setOpenExternalLinks(true);
    }

    connect(ui->doneButton, &QPushButton::clicked, this, &NewsContent::on_FinishButtonClicked);
}

void NewsContent::on_FinishButtonClicked()
{
    this->close();
}

NewsContent::~NewsContent()
{
    ui->newsContentBrowser->deleteLater();
}