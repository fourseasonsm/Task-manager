#include "loginwindow.h"
#include "registrationwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

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
    sendCredentialsToServer();
    if (socket->waitForReadyRead(3000)) {
        QString response = QString::fromUtf8(socket->readAll()).trimmed();
        if (response == "AUTH_SUCCESS") {
            authenticated = true;
            accept();  // Закрываем окно и разрешаем доступ
        } else if (response == "AUTH_FAILED") {
            QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
        } else {
            QMessageBox::warning(this, "Ошибка", "Некорректный ответ от сервера");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера");
    }
}

void LoginWindow::connectToServer() {
    QString serverIp = "localhost";
    int serverPort = 8000;
    socket->connectToHost(serverIp, serverPort);
    if (socket->waitForConnected(3000)) {
        connectionStatusLabel->setText("Подключено к серверу " + serverIp + ":" + QString::number(serverPort));

    } else {
        connectionStatusLabel->setText("Не удалось подключиться к серверу");
    }
}
