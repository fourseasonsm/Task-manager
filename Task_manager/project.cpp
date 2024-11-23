#include "project.h"
#include "projectwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

Project::Project(const QString &title, const QString &description, QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *projectBox = new QLabel(this);
    projectBox->setStyleSheet("background-color: #a7bfa5;");
    projectBox->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->addWidget(projectBox);

    QVBoxLayout *projectBoxLayout = new QVBoxLayout(projectBox); // Вертикальный слой внутри рамки

    // Стиль полей
    QString lineEditStyle = "background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;";

    // Стиль кнопок
    QString buttonStyle = "background-color: #3b4f2a; color: white; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Отступ сверху
    projectBoxLayout->addSpacing(15);

    // Поле ввода названия задачи
    titleEdit = new QLineEdit(title, this);
    titleEdit->setStyleSheet(lineEditStyle + "font-weight: bold;");
    titleEdit->setFixedHeight(40);
    projectBoxLayout->addWidget(titleEdit);

    // Отступ
    projectBoxLayout->addSpacing(10);

    // Поле ввода описания задачи
    descriptionEdit = new QLineEdit(description, this);
    descriptionEdit->setStyleSheet(lineEditStyle);
    descriptionEdit->setFixedHeight(30);
    projectBoxLayout->addWidget(descriptionEdit);

    // Текст "Подзадачи проекта"
    QLabel *sub_task = new QLabel("Подзадачи проекта", this);
    sub_task->setStyleSheet("color: black; font-size: 16px;");
    projectBoxLayout->addWidget(sub_task);

    // Горизонтальный слой для кнопки "Выполнить"
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Кнопка добавления подзадачи
    subTaskButton = new QPushButton("Добавить задачу", this);
    subTaskButton->setFixedSize(130, 40); // Размер кнопки
    subTaskButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(subTaskButton); // Кнопка выполнения

    buttonLayout->addStretch(); // Заполнитель слева для выравнивания кнопки вправо

    // Кнопка открытия задачи
    openButton = new QPushButton("Открыть", this);
    openButton->setFixedSize(100, 40); // Размер кнопки
    openButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(openButton); // Кнопка выполнения

    // Кнопка выполнения
    doneButton = new QPushButton("Выполнить", this);
    doneButton->setFixedSize(100, 40); // Размер кнопки
    doneButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(doneButton); // Кнопка выполнения

    // Добавляем компоновку кнопок в рамку
    projectBoxLayout->addLayout(buttonLayout);

    // Добавляем рамку в главный слой
    layout->addWidget(projectBox);

    setMinimumSize(400, 250); // Устанавливаем минимальный размер окна

    connect(doneButton, &QPushButton::clicked, this, &Project::markAsDone);
    connect(openButton, &QPushButton::clicked, this, &Project::openProject);
    connect(subTaskButton, &QPushButton::clicked, this, &Project::addSubTask);
}

void Project::markAsDone() {
    this->deleteLater(); // Удаляем задачу
}

void Project::openProject() {

    ProjectWindow *projectWindow = new ProjectWindow(this);

    projectWindow->show();
}

void Project::addSubTask() {

}
