#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

class ProjectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectWindow(QWidget *parent = nullptr);
    ~ProjectWindow();
signals:
    void addtask ();// Сигнал для добавления подзадачи
public slots:
    void textChanged(); // Слот для изменения цвета веса подзадачи
    void on_inviteButton_clicked(); // Слот для приглашения пользователя к подзадаче
    void on_deleteButton_clicked(); // Слот для удаления подзадачи
    void on_saveButton_clicked(); // Слот для сохранения подзадачи
    void on_addButton_clicked(); // Слот для добавление подзадачи

private:
    QLineEdit *title; // Поле для редактирования названия
    QTextEdit *description_tsk; // Поле для редактирования описания
    QLabel *subtasks_tsk;
    QPushButton *addButton;
    QVBoxLayout *subTasksLayout; // Компоновка в виджете проекта
};

#endif // PROJECTWINDOW_H
