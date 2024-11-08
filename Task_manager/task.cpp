// #include "task.h"

// Task::Task(const QString &title, const QString &description, QWidget *parent)
//     : QWidget(parent) {
//     QVBoxLayout *layout = new QVBoxLayout(this);

//     layout->setSpacing(10); // Устанавливаем отступ между элементами
//     layout->setContentsMargins(10, 10, 10, 10); // Устанавливаем отступы вокруг всего layout

//     titleEdit = new QLineEdit(title, this);
//     descriptionEdit = new QLineEdit(description, this);
//     doneButton = new QPushButton("Выполнить", this);

//     // Настройка стиля для виджетов
//     titleEdit->setStyleSheet("color: black; background-color: white; font-size: 14px;"); // Для заголовка
//     descriptionEdit->setStyleSheet("color: black; background-color: white; font-size: 14px;"); // Для описания
//     doneButton->setFixedHeight(30); // Устанавливаем фиксированную высоту кнопки

//     layout->addWidget(titleEdit);
//     layout->addWidget(descriptionEdit);
//     layout->addWidget(doneButton);

//     connect(doneButton, &QPushButton::clicked, this, &Task::markAsDone);
// }

// void Task::markAsDone() {
//     this->deleteLater(); // Удаляем задачу
// }


#include "task.h"
#include "taskwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

Task::Task(const QString &title, const QString &description, QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Создание рамки для обведения полей
    QFrame *frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel); // Устанавливаем стиль рамки
    frame->setStyleSheet("border: 2px solid #3b4f2a; padding: 10px;"); // Устанавливаем цвет и толщину рамки

    QVBoxLayout *frameLayout = new QVBoxLayout(frame); // Вертикальный слой внутри рамки

    // Поле ввода названия задачи
    titleEdit = new QLineEdit(title, this);
    titleEdit->setStyleSheet("color: black; background-color: white; font-weight: bold; font-size: 16px;");
    frameLayout->addWidget(titleEdit);

    // Поле ввода описания задачи
    descriptionEdit = new QLineEdit(description, this);
    descriptionEdit->setStyleSheet("color: black; background-color: white; font-size: 14px;");
    frameLayout->addWidget(descriptionEdit);

    // Горизонтальный слой для кнопки "Выполнить"
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Заполнитель слева для выравнивания кнопки вправо

    // Кнопка открытия задачи
    openButton = new QPushButton("Открыть", this);
    openButton->setFixedSize(100, 40); // Размер кнопки
    openButton->setStyleSheet("background-color: green; color: white;");
    layout->addWidget(frame); // Добавляем рамку с полями
    buttonLayout->addWidget(openButton); // Кнопка выполнения

    // Кнопка выполнения
    doneButton = new QPushButton("Выполнить", this);
    doneButton->setFixedSize(100, 40); // Размер кнопки
    doneButton->setStyleSheet("background-color: green; color: white;");
    layout->addWidget(frame); // Добавляем рамку с полями
    buttonLayout->addWidget(doneButton); // Кнопка выполнения

    // Добавляем компоновку кнопки в компоновку рамки
    frameLayout->addLayout(buttonLayout);

    // Добавляем рамку в главный слой
    layout->addWidget(frame);

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

