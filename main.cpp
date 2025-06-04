#include "loginwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings    settings("NextcloudNewsClient", "client");

    LoginWindow w{nullptr, &settings};
    if (!w.LoggedIn())
    {
        w.show();
    }
    else
    {
        // TODO:
    }
    return a.exec();
}
