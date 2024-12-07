#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "task.h"
#include "project.h"
#include "global.h"

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>

class LoginWindow; // Объявление для работы с LoginWindow

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void createNewTask();
    void createNewProject();
    void on_authLoginButton_clicked();
    void on_logoutButton_clicked();
    void on_regButton_clicked();

private:
    void updateAuthButtons();

    QScrollArea *scrollArea; // Указатель на QScrollArea
    QVBoxLayout *tasksLayout; // Layout для хранения задач
    QVBoxLayout *centerStripeLayout; // Вертикальный слой для центральной полосы

    // Указатели на кнопки
    QPushButton *authLoginButton;
    QPushButton *regButton;
    QPushButton *logoutButton;
};

#endif // MAINWINDOW_HPP
