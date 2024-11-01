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
    frame->setStyleSheet("border: 2px solid gray;"); // Устанавливаем цвет и толщину рамки

    QVBoxLayout *frameLayout = new QVBoxLayout(frame); // Вертикальный слой внутри рамки

    // Поле ввода названия задачи
    titleEdit = new QLineEdit(title, this);
    titleEdit->setStyleSheet("color: black; background-color: white; font-weight: bold; font-size: 16px;");
    frameLayout->addWidget(titleEdit);

    // Поле ввода описания задачи
    descriptionEdit = new QLineEdit(description, this);
    descriptionEdit->setStyleSheet("color: black; background-color: white; font-size: 14px;");
    frameLayout->addWidget(descriptionEdit);

    // Кнопка выполнения
    doneButton = new QPushButton("Выполнить", this);
    doneButton->setStyleSheet("background-color: green; color: white;");
    layout->addWidget(frame); // Добавляем рамку с полями
    frameLayout->addWidget(doneButton); // Кнопка выполнения

    connect(doneButton, &QPushButton::clicked, this, &Task::markAsDone);
}

void Task::markAsDone() {
    this->deleteLater(); // Удаляем задачу
}

