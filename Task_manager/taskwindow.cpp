#include "taskwindow.h"
#include "task.h"
#include "ui_taskwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>


TaskWindow::TaskWindow(QWidget *parent,const QString task_name,const QString task_text)
    : QDialog(parent)
{
    setWindowTitle("Задача"); // Позже изменить
    resize(800, 400);
    QVBoxLayout *taskLayout = new QVBoxLayout(this);

    // Стили
    QString lineEditStyle = "background-color: white; color: black; outline: none; border: none;";
    QString buttonStyle = "background-color: #3b4f2a; color: white; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Белая шапка
    QLabel *taskTopPart = new QLabel();
    taskTopPart->setStyleSheet("background-color: white;");
    taskTopPart->setFixedHeight(50);
    taskTopPart->setAlignment(Qt::AlignCenter);
    taskLayout->addWidget(taskTopPart);

    QHBoxLayout *headerLayout = new QHBoxLayout(taskTopPart); // Горизонтальный слой

    // Название проекта
    title = new QLineEdit("Новая задача", this);
    title->setStyleSheet(lineEditStyle + "font-size: 20px;");
    title->setText(task_name);
    headerLayout->addWidget(title, 0, Qt::AlignTop);

    // Добавляем пустое пространство справа для выравнивания
    headerLayout->addStretch(1);

    // Разделитель
    QFrame *taskSeparator = new QFrame();
    taskSeparator->setFrameShape(QFrame::HLine);
    taskSeparator->setFrameShadow(QFrame::Sunken);
    taskSeparator->setLineWidth(2);
    taskSeparator->setStyleSheet("color: #3b4f2a;");
    taskLayout->addWidget(taskSeparator);

    // Текст "Описание задачи"
    description_tsk = new QTextEdit("Описание задачи");
    description_tsk->setStyleSheet(lineEditStyle + "font-size: 15px;");
    description_tsk->setMaximumHeight(150);
    description_tsk->setText(task_text);
    taskLayout->addWidget(description_tsk);

    taskLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Горизонтальный слой для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Заполнитель слева для выравнивания кнопки вправо

    // Создаём кнопку "Сохранить"
    QPushButton *saveButton = new QPushButton("Сохранить", this);
    saveButton->setStyleSheet(buttonStyle);
    saveButton->setFixedSize(100, 40);
    buttonLayout->addWidget(saveButton);

    // Создаём кнопку "Выполнить"
    QPushButton *doneButton = new QPushButton("Выполнить", this);
    doneButton->setStyleSheet(buttonStyle);
    doneButton->setFixedSize(100, 40);
    buttonLayout->addWidget(doneButton);

    // Создаём кнопку "✖"
    QPushButton *closeButton = new QPushButton("✖", this); // Используем символ крестика
    closeButton->setFixedSize(40, 40);
    closeButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(closeButton);

    taskLayout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &TaskWindow::on_saveButton_clicked);
    connect(doneButton, &QPushButton::clicked, this, &TaskWindow::on_doneButton_clicked);
    connect(closeButton, &QPushButton::clicked, this, &TaskWindow::on_closeButton_clicked);
}

TaskWindow::~TaskWindow()
{
}

void TaskWindow::on_saveButton_clicked() {
    emit Change_task_info(title->text(),description_tsk->toPlainText());
    emit savetask();
}

void TaskWindow::on_doneButton_clicked() {
    emit donetask();
}

void TaskWindow::on_closeButton_clicked() {
    QWidget::close(); // Вызываем метод close базового класса
}
