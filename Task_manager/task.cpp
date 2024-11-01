#include "task.h"

Task::Task(const QString &title, const QString &description, QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setSpacing(10); // Устанавливаем отступ между элементами
    layout->setContentsMargins(10, 10, 10, 10); // Устанавливаем отступы вокруг всего layout

    titleEdit = new QLineEdit(title, this);
    descriptionEdit = new QLineEdit(description, this);
    doneButton = new QPushButton("Выполнить", this);

    // Настройка стиля для виджетов
    titleEdit->setStyleSheet("color: black; background-color: white; font-size: 14px;"); // Для заголовка
    descriptionEdit->setStyleSheet("color: black; background-color: white; font-size: 14px;"); // Для описания
    doneButton->setFixedHeight(30); // Устанавливаем фиксированную высоту кнопки

    layout->addWidget(titleEdit);
    layout->addWidget(descriptionEdit);
    layout->addWidget(doneButton);

    connect(doneButton, &QPushButton::clicked, this, &Task::markAsDone);
}

void Task::markAsDone() {
    this->deleteLater(); // Удаляем задачу
}
