#include "registrationwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

RegistrationWindow::RegistrationWindow(QWidget *parent, QTcpSocket *existingSocket)
    : QDialog(parent), socket(existingSocket)  // Инициализация переданным сокетом
{
    /*this->setStyleSheet(
        "background-color: #e0f7e0;"  // неудачная попытка изменения цвета
        "color: #000000;"
        );*/

    setWindowTitle("Регистрация");

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Поле для ввода имени пользователя
    QLabel *loginLabel = new QLabel("Логин:");
    loginEdit = new QLineEdit(this);

    QLabel *emailLabel = new QLabel("Почта:");
    emailEdit = new QLineEdit(this);

    // Поле для ввода пароля
    QLabel *passwordLabel = new QLabel("Пароль:");
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    QLabel *dpasswordLabel = new QLabel("Повторите пароль:");
    dpasswordEdit = new QLineEdit(this);
    dpasswordEdit->setEchoMode(QLineEdit::Password);    // Скрыть пароль

    // Кнопка регистрации
    registerButton = new QPushButton("Зарегистрироваться", this);
    connect(registerButton, &QPushButton::clicked, this, &RegistrationWindow::on_registerButton_clicked);

    // Добавляем виджеты в основной layout
    layout->addWidget(loginLabel);
    layout->addWidget(loginEdit);
    layout->addWidget(emailLabel);
    layout->addWidget(emailEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(dpasswordLabel);
    layout->addWidget(dpasswordEdit);
    layout->addWidget(registerButton);

    setLayout(layout); // Устанавливаем основной layout для диалогового окна
}

void RegistrationWindow::on_registerButton_clicked() {
    registerUser();
    if (socket->waitForReadyRead(3000)) {
        QString response = QString::fromUtf8(socket->readAll()).trimmed();
        if (response == "REGISTER_SUCCESS") {
            QMessageBox::information(this, "Регистрация", "Регистрация прошла успешно");
        } else {
            QMessageBox::warning(this, "Ошибка", "Регистрация не удалась");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера");
    }
}

void RegistrationWindow::registerUser() {
    QString login = loginEdit->text();
    QString email = emailEdit->text();
    QString password = passwordEdit->text();

    // Проверка на то, что все поля заполнены
    if (login.isEmpty()||password.isEmpty()||email.isEmpty()) {
        QMessageBox::warning(this, "Неверные данные", "Пустые поля");
    }
    // Простейшая проверка на длину пароля
    else if (passwordEdit->text().length() < 8) {
        QMessageBox::warning(this, "Неверные данные", "Слишком короткий пароль");
    }
    // Проверка повторного ввода пароля
    else if (passwordEdit->text()!= dpasswordEdit->text()) {
        QMessageBox::warning(this, "Неверные данные", "Неправильный повторный ввод пароля");
    }
    else {
        QString credentials = "REGISTER:" + login + ":" + email + ":" + password;

        if (socket->state() == QAbstractSocket::ConnectedState) {
            socket->write(credentials.toUtf8() + "\n");
        }
    }
}
