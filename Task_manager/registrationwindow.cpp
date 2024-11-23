#include "registrationwindow.h"
#include <QGraphicsDropShadowEffect>
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
    resize(300, 300);

    // Установка фона для всего окна   // удачная попытка :)))
    setStyleSheet("background-color: #a7bfa5;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Отступ сверху
    layout->addSpacing(10);

    // Заголовок
    QLabel *title = new QLabel("Регистрация");
    title->setStyleSheet("color: black; font-size: 30px;"); // font-family: 'Noto Sans' - заеб какой-то пока со шрифтами
    layout->addWidget(title, 0, Qt::AlignTop | Qt::AlignCenter);

    // Отступ
    layout->addSpacing(10);;

    // Стиль полей
    QString lineEditStyle = "background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;";

    // Поле для ввода имени пользователя
    loginEdit = new QLineEdit(this);
    loginEdit->setFixedSize(230, 30);
    loginEdit->setPlaceholderText("Введите логин"); // Текст-заполнитель
    loginEdit->setStyleSheet(lineEditStyle);

    // Поле для ввода почты
    emailEdit = new QLineEdit(this);
    emailEdit->setFixedSize(230, 30);
    emailEdit->setPlaceholderText("Введите почту"); // Текст-заполнитель
    emailEdit->setStyleSheet(lineEditStyle);

    // Поле для ввода пароля
    passwordEdit = new QLineEdit(this);
    passwordEdit->setFixedSize(230, 30);
    passwordEdit->setPlaceholderText("Придумайте пароль"); // Текст-заполнитель
    passwordEdit->setStyleSheet(lineEditStyle);
    passwordEdit->setEchoMode(QLineEdit::Password);

    // Поле для повторного ввода пароля
    dpasswordEdit = new QLineEdit(this);
    dpasswordEdit->setFixedSize(230, 30);
    dpasswordEdit->setPlaceholderText("Повторите пароль"); // Текст-заполнитель
    dpasswordEdit->setStyleSheet(lineEditStyle);
    dpasswordEdit->setEchoMode(QLineEdit::Password);    // Скрыть пароль

    // Стиль кнопок
    QString buttonStyle = "background-color: #3b4f2a; color: white; font-weight: bold; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Кнопка регистрации
    registerButton = new QPushButton("Зарегистрироваться", this);
    registerButton->setStyleSheet(buttonStyle);
    addShadowEffect(registerButton); // Добавляем тень
    connect(registerButton, &QPushButton::clicked, this, &RegistrationWindow::on_registerButton_clicked);

    // Добавляем виджеты в основной layout
    layout->addWidget(loginEdit, 0, Qt::AlignCenter);

    layout->addWidget(emailEdit, 0, Qt::AlignCenter);

    layout->addWidget(passwordEdit, 0, Qt::AlignCenter);

    layout->addWidget(dpasswordEdit, 0, Qt::AlignCenter);

    layout->addWidget(registerButton, 0, Qt::AlignBottom | Qt::AlignCenter);

    setLayout(layout); // Устанавливаем основной layout для диалогового окна
}

// Функция для добавления тени кнопке
void RegistrationWindow::addShadowEffect(QWidget *widget) {
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(widget);
    shadowEffect->setBlurRadius(10);                // Радиус размытия
    shadowEffect->setOffset(3, 3);                  // Смещение тени
    shadowEffect->setColor(QColor(50, 50, 50));     // Цвет тени
    widget->setGraphicsEffect(shadowEffect);
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
