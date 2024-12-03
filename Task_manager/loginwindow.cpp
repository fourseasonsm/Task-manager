#include "loginwindow.h"
#include "registrationwindow.h"

#include <QGraphicsDropShadowEffect>
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
    setWindowTitle("Авторизация");
    resize(300, 300);

    // Установка фона для всего окна
    setStyleSheet("background-color: #a7bfa5;");

    // Главный компоновщик
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Отступ сверху
    layout->addSpacing(10);

    // Заголовок
    QLabel *title = new QLabel("Авторизация");
    title->setStyleSheet("color: black; font-size: 30px;"); // font-family: 'Noto Sans' - заеб какой-то пока со шрифтами
    layout->addWidget(title, 0, Qt::AlignTop | Qt::AlignCenter);

    // Отступ
    layout->addSpacing(10);

    // Стиль полей
    QString lineEditStyle = "background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;";

    // Поле логина
    loginLineEdit = new QLineEdit(this);
    loginLineEdit->setFixedSize(230, 30);
    loginLineEdit->setStyleSheet(lineEditStyle);
    loginLineEdit->setPlaceholderText("Введите логин"); // Текст-заполнитель
    layout->addWidget(loginLineEdit, 0, Qt::AlignCenter);

    // Расстояние между полями логина и пароля
    layout->addSpacing(20);

    // Поле пароля
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setFixedSize(230, 30);
    passwordLineEdit->setStyleSheet(lineEditStyle);
    passwordLineEdit->setPlaceholderText("Введите пароль"); // Текст-заполнитель
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordLineEdit, 0, Qt::AlignCenter);

    // Отступ преред кнопками
    layout->addSpacing(15);

    // Стиль кнопок
    QString buttonStyle = "background-color: #3b4f2a; color: white; font-weight: bold; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Кнопка входа которая отправляет json
    QPushButton *authLoginButton = new QPushButton("Войти", this);
    authLoginButton->setFixedSize(150, 35);
    authLoginButton->setStyleSheet(buttonStyle);
    addShadowEffect(authLoginButton); // Добавляем тень

    layout->addWidget(authLoginButton, 0, Qt::AlignCenter | Qt::AlignBottom);
    connect(authLoginButton, &QPushButton::clicked, this, &LoginWindow::on_authLoginButton_clicked);
    connectToServer();

    // Кнопка регистрации
    QPushButton *regButton = new QPushButton("Зарегистрироваться", this);
    regButton->setFixedSize(150, 35);
    regButton->setStyleSheet(buttonStyle);
    addShadowEffect(regButton); // Добавляем тень

    layout->addWidget(regButton, 0, Qt::AlignCenter | Qt::AlignBottom);
    connect(regButton, &QPushButton::clicked, this, &LoginWindow::on_regButton_clicked);

    // Расстояние между сообщением и кнопками
    layout->addSpacing(10);

    // Сообщение о соединении
    connectionStatusLabel = new QLabel("Соединение не установлено", this);
    connectionStatusLabel->setStyleSheet("color: black");
    layout->addWidget(connectionStatusLabel, 0, Qt::AlignCenter | Qt::AlignBottom);

    // Установка главного компоновщика
    setLayout(layout);
}

// Функция для добавления тени кнопке
void LoginWindow::addShadowEffect(QWidget *widget) {
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(widget);
    shadowEffect->setBlurRadius(10);                // Радиус размытия
    shadowEffect->setOffset(3, 3);                  // Смещение тени
    shadowEffect->setColor(QColor(50, 50, 50));     // Цвет тени
    widget->setGraphicsEffect(shadowEffect);
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
    RegistrationWindow *registerWindow = new RegistrationWindow(this);
    registerWindow->show(); // Отображается поверх окна логина, можно потом пофиксить
}

void LoginWindow::on_authLoginButton_clicked() {
    // Проверка на пустые поля
    if (loginLineEdit->text().isEmpty() || passwordLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Логин и пароль не могут быть пустыми");
        return;
    }
    user_login_global =loginLineEdit->text();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8080"); // Замените на ваш URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для авторизации
    QJsonObject json;
    json["action"] = "login"; // Указываем действие
    json["login"] = loginLineEdit->text(); // Замените на ваше поле логина
    json["password"] = passwordLineEdit->text(); // Замените на ваше поле пароля

    // Преобразуем JSON объект в документ и выводим его в консоль для отладки
    QJsonDocument jsonDoc(json);

    // Отправляем POST запрос
    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

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
                // Сохраняем адрес и порт нового сервера
                QJsonObject serverObject = jsonObject["server"].toObject();
                QString newHost = serverObject["host"].toString();
                int newPort = serverObject["port"].toInt();

                // Сохраняем для использования в Task
                smallServerUrl = QString("http://%1:%2").arg(newHost).arg(newPort);
                accept();  // Закрываем окно и разрешаем доступ
            } else {
                QMessageBox::warning(this, "Ошибка", message);
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера: " + reply->errorString());
        }
        reply->deleteLater(); // Освобождаем память
    });

    // Обрабатываем ошибки сети
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply]() {
        QMessageBox::warning(this, "Ошибка", "Ошибка сети: " + reply->errorString());
    });
}

QString LoginWindow::getSmallServerUrl() const {
    return smallServerUrl;
}

void LoginWindow::connectToServer() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8080"); // Замените на ваш URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для авторизации
    QJsonObject json;
    json["action"] = "status"; // Указываем действие
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
            if (message == "Server live!") {
                connectionStatusLabel->setText("Подключено к серверу ");
            } else {
                QMessageBox::warning(this, "Ошибка", message);
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера: " + reply->errorString());
        }
        reply->deleteLater(); // Освобождаем память
    });
}

