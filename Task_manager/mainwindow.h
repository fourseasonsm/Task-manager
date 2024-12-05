#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "task.h"
#include "project.h"

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>

class MainWindow : public QWidget {
    Q_OBJECT
public:
    //поле для хранения адреса обрабатывающего сервера, которое передается в конструктор Task
    QString smallServerUrl;
    MainWindow(QWidget *parent = nullptr);

private slots:
    void createNewTask();
    void createNewProject();
    void on_authLoginButton_clicked();
    void on_regButton_clicked();
    void on_logoutButton_clicked();
    void updateAuthButtons();

private:
    QScrollArea *scrollArea; // Указатель на QScrollArea
    QVBoxLayout *tasksLayout; // Layout для хранения задач
    QVBoxLayout *centerStripeLayout; // Вертикальный слой для центральной полосы
    QPushButton *authLoginButton;
    QPushButton *regButton;
    QPushButton *logoutButton;
};

#endif // MAINWINDOW_HPP
