#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class RegistrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr);

private slots:
    void on_registerButton_clicked(); // Слот для обработки нажатия кнопки регистрации

private:
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    QPushButton *registerButton;

    bool validate_input(); // Метод для валидации введенных данных
};

#endif // REGISTRATIONWINDOW_H
