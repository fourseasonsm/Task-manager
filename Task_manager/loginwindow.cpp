#include "loginwindow.h"
#include "registrationwindow.h"
#include <QMessageBox>
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

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , authenticated(false)
    , socket(new QTcpSocket(this))  // Инициализация сокета
{
    // Поле логина
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *loginLabel = new QLabel("Логин:", this);
    layout->addWidget(loginLabel);

    loginLineEdit = new QLineEdit(this);
    layout->addWidget(loginLineEdit);

    // Поле пароля
    QLabel *passwordLabel = new QLabel("Пароль:", this);
    layout->addWidget(passwordLabel);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordLineEdit);

    connectionStatusLabel = new QLabel("Соединение не установлено", this);
    layout->addWidget(connectionStatusLabel);

    QPushButton *authLoginButton = new QPushButton("Войти", this);
    layout->addWidget(authLoginButton);
    connect(authLoginButton, &QPushButton::clicked, this, &LoginWindow::on_authLoginButton_clicked);
    connectToServer();

    // Кнопка регистрации
    QPushButton *regButton = new QPushButton("Зарегистрироваться", this);
    layout->addWidget(regButton);
    connect(regButton, &QPushButton::clicked, this, &LoginWindow::on_regButton_clicked);
    setLayout(layout);
}

LoginWindow::~LoginWindow() {
    socket->disconnectFromHost();
}

void LoginWindow::sendCredentialsToServer() {
    QString login = loginLineEdit->text();
    QString password = passwordLineEdit->text();
    QString credentials = "LOGIN:" + login + ":" + password;

    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(credentials.toUtf8() + "\n");
    }
}

QTcpSocket* LoginWindow::getSocket() {
    return socket;
}

// Нажатие на кнопку для перехода к окну регистрации
void LoginWindow::on_regButton_clicked()
{
    registerWindow = new RegistrationWindow(this, getSocket());  // Передаем сокет
    registerWindow->show(); // Отображается поверх оккна логина, можно потом пофиксить
}

void LoginWindow::on_authLoginButton_clicked() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8080"); // Замените на ваш URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для авторизации
    QJsonObject json;
    json["action"] = "login"; // Указываем действие
    json["username"] = loginLineEdit->text(); // Замените на ваше поле логина
    json["password"] = passwordLineEdit->text(); // Замените на ваше поле пароля

    // Отправляем POST запрос
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    // Обрабатываем ответ
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll()).trimmed();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();

            // Проверяем сообщение от сервера
            QString message = jsonObject["message"].toString();
            if (message == "Login successful!") {
                authenticated = true;
                accept();  // Закрываем окно и разрешаем доступ
            } else {
                QMessageBox::warning(this, "Ошибка", message);
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера: " + reply->errorString());
        }
        reply->deleteLater(); // Освобождаем память
    });
}

void LoginWindow::connectToServer() {
    QString serverIp = "localhost";
<<<<<<< HEAD
    int serverPort = 51138;

=======
    int serverPort = 8000;
>>>>>>> de6eec12417335b64cafc028f5d82ba561aeb9fa
    socket->connectToHost(serverIp, serverPort);
    if (socket->waitForConnected(3000)) {
        connectionStatusLabel->setText("Подключено к серверу " + serverIp + ":" + QString::number(serverPort));

    } else {
        connectionStatusLabel->setText("Не удалось подключиться к серверу");
    }
}
