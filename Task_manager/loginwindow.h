#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QTcpSocket>
#include "registrationwindow.h"

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    QTcpSocket* getSocket();  // Метод для получения сокета

private slots:
    void connectToServer();
    void on_authLoginButton_clicked();
    void sendCredentialsToServer();
    void on_regButton_clicked();


private:
    bool authenticated;
    QLineEdit *loginLineEdit;     // Declare login line edit
    QLineEdit *passwordLineEdit;  // Declare password line edit
    QTcpSocket *socket;  // Добавляем сокет
    RegistrationWindow *registerWindow;
    QLabel *connectionStatusLabel; // Метка для отображения состояния подключения
};


