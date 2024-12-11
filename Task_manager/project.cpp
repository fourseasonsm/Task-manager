#include "project.h"
#include "projectwindow.h"
#include "mainwindow.h"
#include "invitewindow.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QScrollArea>
#include <QTextEdit>
#include <QIntValidator>


Project::Project(QWidget *parent)
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
    projectBoxLayout->addSpacing(10);

    // Поле ввода названия проекта
    titleEdit = new QLineEdit(this);
    titleEdit->setStyleSheet(lineEditStyle + "font-weight: bold;");
    titleEdit->setFixedHeight(30);
    titleEdit->setPlaceholderText("Название проекта");
    projectBoxLayout->addWidget(titleEdit);

    // Отступ
    projectBoxLayout->addSpacing(10);

    // Поле ввода описания проекта
    descriptionEdit = new QTextEdit(this);
    descriptionEdit->setStyleSheet(lineEditStyle);
    descriptionEdit->setFixedHeight(70);
    descriptionEdit->setPlaceholderText("Описание проекта");
    projectBoxLayout->addWidget(descriptionEdit);

    // Текст "Подзадачи проекта"
    QLabel *sub_task = new QLabel("Подзадачи проекта", this);
    sub_task->setStyleSheet("color: black; font-size: 16px;");
    projectBoxLayout->addWidget(sub_task);

    // Создаем контейнер для подзадач
    QWidget *subTasksContainer = new QWidget(this);
    subTasksLayout = new QVBoxLayout(subTasksContainer);  // Вертикальная компоновка для подзадач
    subTasksContainer->setLayout(subTasksLayout);

    // Оборачиваем контейнер для подзадач в QScrollArea
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(subTasksContainer);  // Устанавливаем контейнер с подзадачами в QScrollArea
    scrollArea->setWidgetResizable(true);  // Устанавливаем, чтобы содержимое растягивалось
    projectBoxLayout->addWidget(scrollArea);

    // Горизонтальный слой для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Кнопка добавления подзадачи
    subTaskButton = new QPushButton("Добавить задачу", this);
    subTaskButton->setFixedSize(130, 40);
    subTaskButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(subTaskButton);

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

    // Кнопка сохранения
    saveButton = new QPushButton("Сохранить", this);
    saveButton->setFixedSize(100, 40);
    saveButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(saveButton);

    // Кнопка закрытия окна (Рената может её облагородить)
    closeButton = new QPushButton("✖", this); // Используем символ крестика
    closeButton->setFixedSize(40, 40);
    closeButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(closeButton);

    // Добавляем компоновку кнопок в рамку
    projectBoxLayout->addLayout(buttonLayout);

    // Добавляем рамку в главный слой
    layout->addWidget(projectBox);

    setMinimumSize(400, 350); // Устанавливаем минимальный размер окна

    // Разрешение изменения размера окна
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(doneButton, &QPushButton::clicked, this, &Project::markAsDone);
    connect(openButton, &QPushButton::clicked, this, &Project::openProject);
    connect(subTaskButton, &QPushButton::clicked, this, &Project::addSubTask);
}

void Project::markAsDone() {
    this->deleteLater(); // Удаляем задачу
}
void Project::openProject() {

    ProjectWindow *projectWindow = new ProjectWindow(this);
    connect(projectWindow, SIGNAL(savetask()), this, SLOT(addSubTask()));

    projectWindow->show();
}

void Project::saveProject() {

}

void Project::addSubTask() {
    // Создаём горизонтальный слой для подзадачи
    QHBoxLayout *subTaskLayout = new QHBoxLayout();

    // Создаём новое поле ввода для подзадачи
    QLineEdit *newSubTask = new QLineEdit(this);
    newSubTask->setPlaceholderText("Подзадача");
    newSubTask->setFixedHeight(22);
    newSubTask->setStyleSheet("background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;");

    // Поле для ввода веса подзадачи
    QLineEdit *subTaskWeight = new QLineEdit(this);
    subTaskWeight->setValidator(new QIntValidator(1, 10, this));
    subTaskWeight->setPlaceholderText("1-10");
    subTaskWeight->setFixedSize(25, 22);
    subTaskWeight->setAlignment(Qt::AlignCenter);
    subTaskWeight->setStyleSheet("background-color: #e1f0db; color: black; font-size: 10px; outline: none; border: none;");

    // Создаём кнопку "Пригласить"
    QPushButton *inviteButton = new QPushButton("Пригласить", this);
    inviteButton->setStyleSheet("background-color: #3b4f2a; color: white; font-size: 12px; outline: none; border: none; border-radius: 5px; padding: 3px 8px;");
    inviteButton->setFixedHeight(22);

    // Добавляем поле ввода и кнопку в горизонтальный слой
    subTaskLayout->addWidget(newSubTask);
    subTaskLayout->addWidget(subTaskWeight);
    subTaskLayout->addWidget(inviteButton);

    // Добавляем подзадачу в контейнер
    subTasksLayout->addLayout(subTaskLayout);

    // Обновляем отображение
    updateGeometry();

    // Подключаем сигнал измененния текста к слоту
    connect(subTaskWeight, &QLineEdit::textChanged, this, &Project::textChanged);

    // Подключаем сигнал кнопки "Пригласить" к слоту
    connect(inviteButton, &QPushButton::clicked, this, &Project::on_inviteButton_clicked);
}

void Project::on_inviteButton_clicked() {

    InviteWindow *inviteWindow = new InviteWindow(this);
    inviteWindow->show();
}

void Project::textChanged() {
    QLineEdit *subTaskWeight = qobject_cast<QLineEdit*>(sender());
    if (!subTaskWeight) return;

    bool ok;
    int value = subTaskWeight->text().toInt(&ok);

    if (ok && value >= 1 && value <= 10) {
        QColor color;

        if (value == 1) {
            color = QColor(90, 235, 71);  // Зеленый для 1
        } else if (value == 2) {
            color = QColor(161, 235, 71);  // для 2
        } else if (value == 3) {
            color = QColor(221, 235, 71);  // для 3
        } else if (value == 4) {
            color = QColor(235, 191, 71);  // для 4
        } else if (value == 5) {
            color = QColor(235, 167, 71);  // для 5
        } else if (value == 6) {
            color = QColor(235, 145, 71);  // для 6
        } else if (value == 7) {
            color = QColor(235, 126, 71);  // для 7
        } else if (value == 8) {
            color = QColor(235, 107, 71);  // для 8
        } else if (value == 9) {
            color = QColor(235, 88, 71);  // для 9
        } else if (value == 10) {
            color = QColor(235, 71, 71);  // для 10
        } else {
            color = QColor(255, 255, 255);  // Белый для остальных значений
        }

        // Применяем цвет фона
        subTaskWeight->setStyleSheet(QString("background-color: %1; color: black; font-size: 10px; outline: none; border: none;").arg(color.name()));
    } else {
        subTaskWeight->setStyleSheet("background-color: #e1f0db; color: black; font-size: 10px; outline: none; border: none;");  // Если введено неверное число, оставляем белый фон
    }
}
