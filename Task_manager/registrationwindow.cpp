#include "registrationwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

RegistrationWindow::RegistrationWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Registration");

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Поле для ввода имени пользователя
    QLabel *loginLabel = new QLabel("Логин:");
    loginEdit = new QLineEdit(this);

    // Поле для ввода пароля
    QLabel *passwordLabel = new QLabel("Пароль:");
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password); // Скрыть пароль

    // Кнопка регистрации
    registerButton = new QPushButton("Зарегистрироваться", this);
    connect(registerButton, &QPushButton::clicked, this, &RegistrationWindow::on_registerButton_clicked);

    // Добавляем виджеты в основной layout
    layout->addWidget(loginLabel);
    layout->addWidget(loginEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(registerButton);

    setLayout(layout); // Устанавливаем основной layout для диалогового окна
}

void RegistrationWindow::on_registerButton_clicked() {
    // Проверка введенных данных
    if (validate_input()) {
        // Если данные корректны, можно выполнить логику регистрации (сохранение данных, запросы к базе и т.д.)
        QMessageBox::information(this, "Success", "Вы успешно зарегистрировались!");
        accept(); // Закрыть окно после успешной регистрации
    } else {
        // В случае некорректных данных показать сообщение об ошибке
        QMessageBox::warning(this, "Error", "Некорректный ввод данных.");
    }
}

bool RegistrationWindow::validate_input() {
    // Проверка на то, что все поля заполнены
    if (loginEdit->text().isEmpty() || passwordEdit->text().isEmpty()) {
        return false;
    }

    // Простейшая проверка на длину пароля
    if (passwordEdit->text().length() < 6) {
        return false;
    }

    return true; // Все проверки пройдены, данные корректны
}
