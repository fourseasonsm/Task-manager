#include "projectwindow.h"
#include "project.h"
#include "ui_projectwindow.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QScrollArea>

ProjectWindow::ProjectWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Проект"); // Позже изменить
    resize(800, 400);
    QVBoxLayout *taskLayout = new QVBoxLayout(this);

    // Стиль полей и кнопой
    QString lineEditStyle = "background-color: white; color: black; outline: none; border: none;";
    QString buttonStyle = "background-color: #3b4f2a; color: white; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Белая шапка
    QLabel *taskTopPart = new QLabel();
    taskTopPart->setStyleSheet("background-color: white;");
    taskTopPart->setFixedHeight(32);
    taskTopPart->setAlignment(Qt::AlignCenter);
    taskLayout->addWidget(taskTopPart);

    QHBoxLayout *headerLayout = new QHBoxLayout(taskTopPart); // Горизонтальный слой

    // Название проекта
    title = new QLineEdit(this);
    title->setStyleSheet(lineEditStyle + "font-size: 20px;");
    title->setFixedHeight(30);
    title->setPlaceholderText("Название проекта");
    headerLayout->addWidget(title);

    // Разделитель
    QFrame *taskSeparator = new QFrame();
    taskSeparator->setFrameShape(QFrame::HLine);
    taskSeparator->setFrameShadow(QFrame::Sunken);
    taskSeparator->setLineWidth(2);
    taskSeparator->setStyleSheet("color: #3b4f2a;");
    taskLayout->addWidget(taskSeparator);

    // Текст "Описание проекта"
    description_tsk = new QTextEdit(this);
    description_tsk->setStyleSheet(lineEditStyle + "font-size: 15px;");
    description_tsk->setFixedHeight(70);
    description_tsk->setPlaceholderText("Описание проекта");
    taskLayout->addWidget(description_tsk);

    // Отступ
    taskLayout->addSpacing(10);

    // Создаём горизонтальный слой для подзадачи
    QHBoxLayout *subLayout = new QHBoxLayout();

    // Текст "Подзадачи проекта"
    QLabel *subtasks_tsk = new QLabel("Подзадачи проекта", this);
    subtasks_tsk->setStyleSheet("background-color: transparent; color: black; text-decoration: underline; font-size: 14px; outline: none; border: none;");
    subLayout->addWidget(subtasks_tsk);

    // Кнопка "Добавить подзадачу"
    QPushButton *addButton = new QPushButton("Добавить подзадачу");
    addButton->setStyleSheet("background-color: #3b4f2a; color: white; font-size: 12px; outline: none; border: none; border-radius: 5px; padding: 3px 8px;");
    addButton->setFixedSize(130, 22);
    subLayout->addWidget(addButton);

    subLayout->addStretch();

    // Отступ
    taskLayout->addSpacing(10);

    // Создаем контейнер для подзадач
    QWidget *subContainer = new QWidget(this);
    subTasksLayout = new QVBoxLayout(subContainer);  // Вертикальная компоновка для подзадач
    subContainer->setLayout(subTasksLayout);

    // Оборачиваем контейнер для подзадач в QScrollArea
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(subContainer);  // Устанавливаем контейнер с подзадачами в QScrollArea
    scrollArea->setWidgetResizable(true);  // Устанавливаем, чтобы содержимое растягивалось

    taskLayout->addLayout(subLayout);
    taskLayout->addWidget(scrollArea);

    // taskLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(addButton, &QPushButton::clicked, this, &ProjectWindow::on_addButton_clicked);
}

ProjectWindow::~ProjectWindow()
{
}

void ProjectWindow::on_addButton_clicked() {
    // Создаём горизонтальный слой для подзадачи
    QHBoxLayout *subTaskLayout = new QHBoxLayout();

    // Создаём новое поле ввода для подзадачи
    QLineEdit *newSubTask = new QLineEdit(this);
    newSubTask->setPlaceholderText("Подзадача");
    newSubTask->setFixedHeight(22);
    newSubTask->setStyleSheet("background-color: white; color: black; font-size: 14px; outline: none; border: none;");

    // Поле для ввода веса подзадачи
    QLineEdit *subTaskWeight = new QLineEdit(this);
    subTaskWeight->setValidator(new QIntValidator(1, 10, this));
    subTaskWeight->setPlaceholderText("1-10");
    subTaskWeight->setFixedSize(25, 22);
    subTaskWeight->setAlignment(Qt::AlignCenter);
    subTaskWeight->setStyleSheet("background-color: white; color: black; font-size: 10px; outline: none; border: none;");

    // Приглашенный пользователь
    QLineEdit *user_tsk = new QLineEdit("Имя пользователя");
    user_tsk->setStyleSheet("background-color: transparent; color: black; font-size: 15px;");
    user_tsk->setFixedSize(235, 22);

    // Создаём горизонтальный слой для подзадачи
    QHBoxLayout *subButtonsLayout = new QHBoxLayout();

    subButtonsLayout->addStretch();

    // Создаём кнопку "Пригласить"
    QPushButton *inviteButton = new QPushButton("Пригласить", this);
    inviteButton->setStyleSheet("background-color: #3b4f2a; color: white; font-size: 12px; outline: none; border: none; border-radius: 5px; padding: 3px 8px;");
    inviteButton->setFixedHeight(22);

    // Создаём кнопку "Сохранить"
    QPushButton *saveButton = new QPushButton("Сохранить", this);
    saveButton->setStyleSheet("background-color: #3b4f2a; color: white; font-size: 12px; outline: none; border: none; border-radius: 5px; padding: 3px 8px;");
    saveButton->setFixedHeight(22);

    // Создаём кнопку "Удалить"
    QPushButton *deleteButton = new QPushButton("Удалить", this);
    deleteButton->setStyleSheet("background-color: #3b4f2a; color: white; font-size: 12px; outline: none; border: none; border-radius: 5px; padding: 3px 8px;");
    deleteButton->setFixedHeight(22);

    // Добавляем поле ввода и кнопку в горизонтальный слой
    subTaskLayout->addWidget(newSubTask);
    subTaskLayout->addWidget(subTaskWeight);
    subTaskLayout->addWidget(user_tsk);
    subButtonsLayout->addWidget(inviteButton);
    subButtonsLayout->addWidget(saveButton);
    subButtonsLayout->addWidget(deleteButton);

    // Добавляем подзадачу в контейнер
    subTasksLayout->addLayout(subTaskLayout);
    subTasksLayout->addLayout(subButtonsLayout);

    // Обновляем отображение
    updateGeometry();

    // Подключаем сигнал измененния текста к слоту
    connect(subTaskWeight, &QLineEdit::textChanged, this, &ProjectWindow::textChanged);

    // Подключаем сигнал кнопки "Пригласить" к слоту
    connect(inviteButton, &QPushButton::clicked, this, &ProjectWindow::on_inviteButton_clicked);
    connect(saveButton, &QPushButton::clicked, this, &ProjectWindow::on_saveButton_clicked);
    connect(deleteButton, &QPushButton::clicked, this, &ProjectWindow::on_deleteButton_clicked);
}

void ProjectWindow::on_inviteButton_clicked() {

}

void ProjectWindow::on_deleteButton_clicked() {

}

void ProjectWindow::on_saveButton_clicked() {
    emit savetask();
}

void ProjectWindow::textChanged() {
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
