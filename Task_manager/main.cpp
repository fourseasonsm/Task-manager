#include "loginwindow.h"
#include "mainwindow.h"
#include "registrationwindow.h"
#include <QApplication>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

class MyApp : public QApplication {
public:
    MyApp(int &argc, char **argv) : QApplication(argc, argv) {
        sendLoginRequest();
    }

private:
    void sendLoginRequest() {
        if (user_login_global != "k") {
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QUrl url("http://localhost:8080"); // Замените на ваш URL
            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            // Создаем JSON объект с данными для авторизации
            QJsonObject json;
            json["action"] = "logout";
            json["login"] = user_login_global;

            // Преобразуем JSON объект в документ
            QJsonDocument jsonDoc(json);

            // Отправляем POST запрос
            QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

            // Подключаем сигнал finished к слоту
            QObject::connect(reply, &QNetworkReply::finished, [reply]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QString response = QString::fromUtf8(reply->readAll()).trimmed();

                    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
                    QJsonObject jsonObject = jsonResponse.object();

                    // Проверяем сообщение от сервера
                    QString message = jsonObject["message"].toString();
                    if (message == "Logout successful!") {
                        qDebug() << "Логаут успешен";
                    } else {
                        qDebug() << "Логаут не успешен";
                    }
                } else {
                    qDebug() << "Ошибка при получении ответа: " << reply->errorString();
                }
                reply->deleteLater(); // Освобождаем память
                exit (0);
            });
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();


    // LoginWindow loginWindow;
    // if (loginWindow.exec() == QDialog::Accepted) {
    //     MainWindow mainWindow;
    //     mainow.resize(700, 500);
    //     mainWindow.show(); // Или используйте exec(), если нужно модальное окно

    //     return a.exec(); // Запустите основной цикл событий
    // }

    // Если убрать комм, при попытке перейти к окну регистрации все окна закрываютя :(

    // RegistrationWindow registrationWindow;
    // if (registrationWindow.exec() == QDialog::Accepted) {
    //     LoginWindow loginWindow;
    //     loginWindow.show();

    //     return a.exec();
    // }

    app.exec();
    MyApp rer(argc, argv);
    rer.exec();
    return 0;
}
