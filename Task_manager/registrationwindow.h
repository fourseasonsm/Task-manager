#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QTcpSocket>
#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;

class RegistrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationWindow(const QUrl &mainServerUrl, QWidget *parent = nullptr);

    static void addShadowEffect(QWidget *widget);

private slots:
    void on_registerButton_clicked(); // Слот для обработки нажатия кнопки регистрации
    bool registerUser(); // Новый слот для регистрации

private:
    const QUrl &mainServerUrl; // Ссылка на URL сервера
    QLineEdit *loginEdit;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLineEdit *dpasswordEdit;
    QPushButton *registerButton;
};

#endif // REGISTRATIONWINDOW_H
