#include "task.h"
#include "taskwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

Task::Task(const QString &title, const QString &description, QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *taskBox = new QLabel(this);
    taskBox->setStyleSheet("background-color: #a7bfa5;");
    taskBox->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->addWidget(taskBox);

    QVBoxLayout *taskBoxLayout = new QVBoxLayout(taskBox);

    // Стиль полей
    QString lineEditStyle = "background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;";

    // Стиль кнопок
    QString buttonStyle = "background-color: #3b4f2a; color: white; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Отступ сверху
    taskBoxLayout->addSpacing(15);

    // Поле ввода названия задачи
    titleEdit = new QLineEdit(title, this);
    titleEdit->setStyleSheet(lineEditStyle + "font-weight: bold;");
    titleEdit->setFixedHeight(40);
    taskBoxLayout->addWidget(titleEdit);

    // Отступ
    taskBoxLayout->addSpacing(15);

    // Поле ввода описания задачи
    descriptionEdit = new QLineEdit(description, this);
    descriptionEdit->setStyleSheet(lineEditStyle);
    descriptionEdit->setFixedHeight(30);
    taskBoxLayout->addWidget(descriptionEdit);

    // Горизонтальный слой для кнопки "Выполнить"
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Заполнитель слева для выравнивания кнопки вправо

    // Кнопка открытия задачи
    openButton = new QPushButton("Открыть", this);
    openButton->setFixedSize(100, 40);
    openButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(openButton);

    // Кнопка выполнения
    doneButton = new QPushButton("Выполнить", this);
    doneButton->setFixedSize(100, 40);
    doneButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(doneButton);

    // Добавляем компоновку кнопок в рамку
    taskBoxLayout->addLayout(buttonLayout);

    // Добавляем рамку в главный слой
    layout->addWidget(taskBox);

    setMinimumSize(400, 250); // Устанавливаем минимальный размер окна

    connect(doneButton, &QPushButton::clicked, this, &Task::markAsDone);
    connect(openButton, &QPushButton::clicked, this, &Task::openTask);
}

void Task::markAsDone() {
    this->deleteLater(); // Удаляем задачу
}

void Task::openTask() {

    TaskWindow *taskWindow = new TaskWindow(this);

    taskWindow->show();
}

