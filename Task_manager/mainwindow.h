#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QListWidget>

#include "task.h"
#include "project.h"
#include "global.h"

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //поле для хранения адреса обрабатывающего сервера, которое передается в конструктор Task
    QString smallServerUrl;

private slots:
    void createNewTask();
    void createNewProject();
    void on_authLoginButton_clicked();
    void on_regButton_clicked();
    void on_logoutButton_clicked();
    void on_refreshButton_clicked();

private:
    QStringList mainServerUrls;
    QString selectAvailableServer(const QStringList &serverUrls);

    bool isServerAvailable(const QString &serverUrl);
    void updateAuthButtons();
    void updateUserName(QString &newUserName);
    void deleteTasks();
    void updateListOfTask();
    void updateUsersOnline();
    void Load_list_of_tasks();

    QScrollArea *scrollArea; // Указатель на QScrollArea
    QVBoxLayout *tasksLayout; // Layout для хранения задач
    QVBoxLayout *centerStripeLayout; // Вертикальный слой для центральной полосы

    QLabel *user_name; // Поле для хранения имени вошедшего юзера
    QListWidget *usersList;

    // Указатели на кнопки
    QPushButton *authLoginButton;
    QPushButton *regButton;
    QPushButton *logoutButton;
};

#endif // MAINWINDOW_HPP

