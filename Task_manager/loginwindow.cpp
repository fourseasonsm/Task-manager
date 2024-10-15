#include "loginwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , authenticated(false)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *loginLabel = new QLabel("Логин:", this);
    layout->addWidget(loginLabel);

    loginLineEdit = new QLineEdit(this);
    layout->addWidget(loginLineEdit);

    QLabel *passwordLabel = new QLabel("Пароль:", this);
    layout->addWidget(passwordLabel);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password); //ввод пароля не виден
    layout->addWidget(passwordLineEdit);

    QPushButton *authLoginButton = new QPushButton("Войти", this);
    layout->addWidget(authLoginButton);
    connect(authLoginButton, &QPushButton::clicked, this, &LoginWindow::on_authLoginButton_clicked);
    setLayout(layout);
}

LoginWindow::~LoginWindow()
{
}

bool LoginWindow::isAuthenticated() const
{
    return authenticated;
}

void LoginWindow::on_authLoginButton_clicked()
{
    QString login = loginLineEdit->text();
    QString password = passwordLineEdit->text();

    if (login == "111" && password == "111") {
        authenticated = true;
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}
