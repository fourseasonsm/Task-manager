#pragma once

#include <QDialog>
#include <QLineEdit>

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    bool isAuthenticated() const;

private slots:
    void on_authLoginButton_clicked();
    void on_regButton_clicked();

private:
    bool authenticated;
    QLineEdit *loginLineEdit;     // Declare login line edit
    QLineEdit *passwordLineEdit;  // Declare password line edit
};


