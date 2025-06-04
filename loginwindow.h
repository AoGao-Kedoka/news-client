#pragma once
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui
{
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

  public:
    LoginWindow(QWidget *parent = nullptr, QSettings *settings = nullptr);
    ~LoginWindow();
    bool LoggedIn();

  private slots:
    void on_LoginButton_clicked();
    void on_ExitButton_clicked();
    void on_networkReplyFinished(QNetworkReply *reply);

  private:
    void SendPostRequest(QString url);
    void PollAppPassword(QString url, QString token, QString endPoint, QString login);
    void OnAppPasswordSuccess(QString server, QString userName, QString appPassword);
    void JumpToMainApp();

  private:
    std::unique_ptr<Ui::LoginWindow>       ui;
    std::unique_ptr<QNetworkAccessManager> networkManager;
    QNetworkRequest                        request;
    std::unique_ptr<QTimer>                pollTimer;
    QSettings                             *settings;
    QString                                loginSuffix = "/index.php/login/v2";
};
