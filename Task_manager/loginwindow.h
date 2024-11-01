#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QTcpSocket>

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void connectToServer();
    void on_authLoginButton_clicked();
    void sendCredentialsToServer();
    void registerUser(); // Новый слот для регистрации
    void on_registerButton_clicked(); // Новый слот для обработки нажатия на кнопку регистрации

private:
    bool authenticated;
    QLineEdit *loginLineEdit; // Поле ввода логина
    QLineEdit *passwordLineEdit; // Поле ввода пароля
    QTcpSocket *socket; // Сокет для связи с сервером
    QLabel *connectionStatusLabel; // Метка для отображения состояния подключения
};
