#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QTcpSocket>
#include <QUrl>
#include "registrationwindow.h"

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(const QUrl &mainServerUrl, QWidget *parent = nullptr);
    ~LoginWindow();

    static void addShadowEffect(QWidget *widget);
    QString getSmallServerUrl() const;  // Геттер для smallServerUrl

private slots:
    void connectToServer();
    void on_authLoginButton_clicked();
    void on_regButton_clicked();


private:
    const QUrl &mainServerUrl; // Ссылка на URL сервера
    QString smallServerUrl; // Поле для хранения адреса обрабатывающего серврера
    bool authenticated;
    QLineEdit *loginLineEdit;     // Declare login line edit
    QLineEdit *passwordLineEdit;  // Declare password line edit
    RegistrationWindow *registerWindow;
    QLabel *connectionStatusLabel; // Метка для отображения состояния подключения
};


