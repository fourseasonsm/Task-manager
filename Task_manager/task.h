#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

    class Task : public QWidget {
    Q_OBJECT

public:
    Task(QWidget *parent = nullptr);

private slots:
    void markAsDone(); // Слот для пометки задачи как выполненной
    void openTask(); // Слот для открытия задачи в окне
    void saveTask(); // Слот для сохранения задачи

private:
    QLineEdit *titleEdit; // Поле для редактирования названия
    QTextEdit *descriptionEdit; // Поле для редактирования описания
    QPushButton *doneButton; // Кнопка для пометки задачи выполненной
    QPushButton *openButton; // Кнопка для открытия задачи в окне
    QPushButton *saveButton; // Кнопка для сохранения задачи
};

#endif // TASK_H
