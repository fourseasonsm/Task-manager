#ifndef PROJECT_H
#define PROJECT_H

#include "invitewindow.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

    class Project : public QWidget {
    Q_OBJECT

public:
    Project(QWidget *parent = nullptr);
    QString invited_login_temp = "k";

public slots:
    void addSubTask(); // Слот для создания новой задачи
    void invite_user(const QString &invited_user_login); // Слот для приглашения юзера

private slots:
    QString extractSubTasksInfo();
    void on_invite_clicked();
    void markAsDone(); // Слот для пометки задачи как выполненной
    void openProject(); // Слот для открытия задачи в окне
    void textChanged(); // Слот для изменения цвета веса подзадачи
    void saveProject(); // Слот для сохранения задачи

private:
    QLineEdit *titleEdit; // Поле для редактирования названия
    QLabel *invitedUserLabel; // Поле для редактирования названия
    QTextEdit *descriptionEdit; // Поле для редактирования описания
    QPushButton *doneButton; // Кнопка для пометки задачи выполненной
    QPushButton *openButton; // Кнопка для открытия задачи в окне
    QPushButton *saveButton; // Кнопка для сохранения проекта
    QPushButton *inviteButton; // Кнопка для приглашения другого пользователя в проект
    QPushButton *closeButton; // Кнопка для закрытия проекта
    QLineEdit *assignedUser;
    QString smallServerUrl; // Член класса для хранения smallServerUrl
    QString project_id; // Член класса для хранения id таска
    QPushButton *subTaskButton; // Кнопка для добавления подзадачи
    QVBoxLayout *subTasksLayout; // Компоновка в виджете проекта
    QLineEdit subTaskWeight;
};

#endif // PROJECT_H
