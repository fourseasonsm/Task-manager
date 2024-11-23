#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class Task : public QWidget {
    Q_OBJECT

public:
    Task(const QString &title, const QString &description, QWidget *parent = nullptr);

private slots:
    void markAsDone(); // Слот для пометки задачи как выполненной
    void openTask(); // Слот для открытия задачи в окне

private:
    QLineEdit *titleEdit; // Поле для редактирования названия
    QLineEdit *descriptionEdit; // Поле для редактирования описания
    QPushButton *doneButton; // Кнопка для пометки задачи выполненной
    QPushButton *openButton; // Кнопка для открытия задачи в окне
};

#endif // TASK_H
