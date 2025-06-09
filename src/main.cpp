#include "loginwindow.h"
#include "mainwindow.h"
#include "newscontent.h"

#include <QApplication>
#include <QFile>
#include <QNetworkAccessManager>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSettings    settings("NextcloudNewsClient", "client");

    QNetworkAccessManager networkManager;

    LoginWindow loginWindow{nullptr, &settings, &networkManager};
    MainWindow  mainWindow{nullptr, &settings, &networkManager};
    loginWindow.mainWindowHandle = &mainWindow;

    if (!loginWindow.LoggedIn())
    {
        loginWindow.show();
    }
    else
    {
        mainWindow.show();
    }
    return app.exec();
}
