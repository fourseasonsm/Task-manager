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
    passwordLineEdit->setEchoMode(QLineEdit::Password); //ввод пароля не виден
    layout->addWidget(passwordLineEdit);

    // Кнопка входа
    QPushButton *authLoginButton = new QPushButton("Войти", this);
    layout->addWidget(authLoginButton);
    connect(authLoginButton, &QPushButton::clicked, this, &LoginWindow::on_authLoginButton_clicked);

    // Кнопка регистрации
    QPushButton *regButton = new QPushButton("Зарегистрироваться", this);
    layout->addWidget(regButton);
    connect(regButton, &QPushButton::clicked, this, &LoginWindow::on_regButton_clicked);

    setLayout(layout);
}

LoginWindow::~LoginWindow()
{
}

// Нажатие на кнопку для перехода к окну регистрации
void LoginWindow::on_regButton_clicked()
{
    RegistrationWindow *registerWindow = new RegistrationWindow(this);

    registerWindow->show(); // Отображается поверх оккна логина, можно потом пофиксить
}

bool LoginWindow::isAuthenticated() const
{
    return authenticated;
}

// Проверка логина и пароля
void LoginWindow::on_authLoginButton_clicked()
{
    QString login = loginLineEdit->text();
    QString password = passwordLineEdit->text();

    // Текущие логин и пароль для входа 111 и 111
    if (login == "111" && password == "111") {
        authenticated = true;
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}
