#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class Project : public QWidget {
    Q_OBJECT

public:
    Project(const QString &title, const QString &description, QWidget *parent = nullptr);

private slots:
    void addSubTask(); // Слот для создания новой задачи
    void markAsDone(); // Слот для пометки задачи как выполненной
    void openProject(); // Слот для открытия задачи в окне

private:
    QLineEdit *titleEdit; // Поле для редактирования названия
    QLineEdit *descriptionEdit; // Поле для редактирования описания
    QPushButton *doneButton; // Кнопка для пометки задачи выполненной
    QPushButton *openButton; // Кнопка для открытия задачи в окне
    QPushButton *subTaskButton; // Кнопка для добавления подзадачи
};

#endif // PROJECT_H
