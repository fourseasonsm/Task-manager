#include "loginwindow.h"
#include "mainwindow.h"
#include "registrationwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWindow loginWindow;
    if (loginWindow.exec() == QDialog::Accepted) {
        MainWindow mainWindow;
        mainWindow.resize(700, 500);
        mainWindow.show(); // Или используйте exec(), если нужно модальное окно

        return a.exec(); // Запустите основной цикл событий
    }

    // Если убрать комм, при попытке перейти к окну регистрации все окна закрываютя :(

    // RegistrationWindow registrationWindow;
    // if (registrationWindow.exec() == QDialog::Accepted) {
    //     LoginWindow loginWindow;
    //     loginWindow.show();

    //     return a.exec();
    // }

    return 0;
}
