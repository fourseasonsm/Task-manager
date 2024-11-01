#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Client"); // Устанавливаем имя приложения
    MainWindow client; // Создаем объект класса MainWindow
    client.show(); // Показываем окно
    return app.exec();
}
