#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

class ProjectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectWindow(QWidget *parent = nullptr);
    ~ProjectWindow();

public slots:
    void textChanged(); // Слот для изменения цвета веса подзадачи
    void on_inviteButton_clicked(); // Слот для приглашения пользователя к подзадаче
    void on_deleteButton_clicked(); // Слот для удаления подзадачи
    void on_saveButton_clicked(); // Слот для сохранения подзадачи

private:
    QLineEdit *title; // Поле для редактирования названия
    QTextEdit *description_tsk; // Поле для редактирования описания
    QLabel *subtasks_tsk;
};

#endif // PROJECTWINDOW_H
