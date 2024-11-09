#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QTcpSocket>
#include <QDialog>

class QLineEdit;
class QPushButton;

class RegistrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr, QTcpSocket *existingSocket = nullptr);

private slots:
    void on_registerButton_clicked(); // Слот для обработки нажатия кнопки регистрации
    void registerUser(); // Новый слот для регистрации

private:
    QTcpSocket *socket;  // Используем переданный сокет
    QLineEdit *loginEdit;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLineEdit *dpasswordEdit;
    QPushButton *registerButton;
};

#endif // REGISTRATIONWINDOW_H
