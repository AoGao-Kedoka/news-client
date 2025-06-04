#include "loginwindow.h"
#include "./ui_loginwindow.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

LoginWindow::LoginWindow(QWidget *parent, QSettings *settings) :
    QMainWindow(parent), ui(std::make_unique<Ui::LoginWindow>()), settings(settings)
{
    ui->setupUi(this);

    networkManager = std::make_unique<QNetworkAccessManager>();
    pollTimer      = std::make_unique<QTimer>();

    QObject::connect(networkManager.get(), SIGNAL(finished(QNetworkReply *)), this,
                     SLOT(on_networkReplyFinished(QNetworkReply *)));

    QString lastUrl = settings->value("lastUsedUrl", "https://nextcloud.com").toString();
    ui->URLInput->setText(lastUrl);
}

LoginWindow::~LoginWindow()
{}

// https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html#login-flow-v2
void LoginWindow::on_LoginButton_clicked()
{
    QString url = ui->URLInput->text();
    SendPostRequest(url);
}

void LoginWindow::on_networkReplyFinished(QNetworkReply *reply)
{
    QString answer = reply->readAll();

    QJsonDocument jsonDoc  = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject   jsonObj  = jsonDoc.object();
    QString       loginUrl = jsonObj.value("login").toString();
    QJsonObject   pollObj  = jsonObj.value("poll").toObject();

    if (answer.contains("server"))
    {
        OnAppPasswordSuccess(jsonObj.value("server").toString(), jsonObj.value("loginName").toString(),
                             jsonObj.value("appPassword").toString());
        pollTimer->stop();
    }
    else if (!loginUrl.isEmpty())
    {
        QDesktopServices::openUrl(QUrl(loginUrl));
        qDebug() << "Login URL opened in browser: " << loginUrl;

        QString url = ui->URLInput->text();
        PollAppPassword(url, pollObj.value("token").toString(), pollObj.value("endpoint").toString(),
                        jsonObj.value("login").toString());
    }

    reply->deleteLater();
}

void LoginWindow::SendPostRequest(QString url)
{
    request.setUrl(QUrl(url + loginSuffix));
    networkManager->post(request, QByteArray());

    QSettings settings("Login");
    settings.setValue("lastUsedUrl", url);
}

void LoginWindow::PollAppPassword(QString url, QString token, QString endPoint, QString login)
{
    qDebug() << "Endpoint" << endPoint;
    pollTimer->start(1000);
    QObject::connect(pollTimer.get(), &QTimer::timeout, [=]() {
        request.setUrl(QUrl(endPoint));

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QJsonObject body;
        body["token"] = token;

        QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    });
}

void LoginWindow::OnAppPasswordSuccess(QString server, QString userName, QString appPassword)
{
    settings->setValue("server", server);
    settings->setValue("userName", userName);
    settings->setValue("appPassword", appPassword);
    JumpToMainApp();
}

void LoginWindow::JumpToMainApp()
{
    this->close();
}

bool LoginWindow::LoggedIn()
{
    QString appPassword = settings->value("appPassword", "").toString();
    return !appPassword.isEmpty();
}

void LoginWindow::on_ExitButton_clicked()
{
    qApp->exit();
}
