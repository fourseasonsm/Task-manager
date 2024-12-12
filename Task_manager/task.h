#ifndef TASK_H
#define TASK_H

#include "loginwindow.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class Task : public QWidget {
    Q_OBJECT

public:
    Task(const QString &smallServerUrl, QWidget *parent = nullptr); // Конструктор с параметром smallServerUrl
    Task(const QString &smallServerUrl, QWidget *parent, QString task_name, QString task_text); // Конструктор с названием и текстом
    QString task_id; // Член класса для хранения id таска
    QLineEdit *titleEdit; // Поле для редактирования названия
    QTextEdit *descriptionEdit; // Поле для редактирования описания
public slots:
    void markAsDone(); // Слот для пометки задачи как выполненной
    void saveTask(); // Слот для сохранения задачи



private slots:
    void openTask(); // Слот для открытия задачи в окне
    void close();
    void change_task_info(QString task_name, QString task_text);

private:
    QString smallServerUrl; // Член класса для хранения smallServerUrl
    QPushButton *doneButton; // Кнопка для пометки задачи выполненной
    QPushButton *openButton; // Кнопка для открытия задачи в окне
    QPushButton *saveButton; // Кнопка для сохранения задач
    QPushButton *closeButton; // Кнопка для закрытия задачи

};

#endif // TASK_H
