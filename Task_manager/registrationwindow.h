#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QTcpSocket>
#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;

extern QString user_login_global; // глобальная переменная которая которая хранит юзера

class RegistrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr, QTcpSocket *existingSocket = nullptr);

    static void addShadowEffect(QWidget *widget);

private slots:
    void on_registerButton_clicked(); // Слот для обработки нажатия кнопки регистрации
    bool registerUser(); // Новый слот для регистрации

private:
    QTcpSocket *socket;  // Используем переданный сокет
    QLineEdit *loginEdit;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLineEdit *dpasswordEdit;
    QPushButton *registerButton;
};

#endif // REGISTRATIONWINDOW_H
