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
#include <QListWidget>

class LoginWindow; // Объявление для работы с LoginWindow

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
    void on_refreshButton_clicked();
    void Load_list_of_tasks();
    void updateUsersOnline(); 
    QString selectAvailableServer(const QStringList &serverUrls);

private:
    //Поле для хранения имени вошедшего юзера
    QLabel *user_name;
    QStringList mainServerUrls;

    bool isServerAvailable(const QString &serverUrl);
    void updateAuthButtons();
    void updateUserName(QString &newUserName);

    QScrollArea *scrollArea; // Указатель на QScrollArea
    QVBoxLayout *tasksLayout; // Layout для хранения задач
    QVBoxLayout *centerStripeLayout; // Вертикальный слой для центральной полосы
    QListWidget *usersList;

    // Указатели на кнопки
    QPushButton *authLoginButton;
    QPushButton *regButton;
    QPushButton *logoutButton;
};

#endif // MAINWINDOW_HPP
