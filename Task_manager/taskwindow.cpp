#include "taskwindow.h"
#include "task.h"
#include "ui_taskwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>


TaskWindow::TaskWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Задача"); // Позже изменить
    resize(800, 400);
    QVBoxLayout *taskLayout = new QVBoxLayout(this);

    // Белая шапка
    QLabel *taskTopPart = new QLabel();
    taskTopPart->setStyleSheet("background-color: white;");
    taskTopPart->setFixedHeight(50);
    taskTopPart->setAlignment(Qt::AlignCenter);
    taskLayout->addWidget(taskTopPart);

    QHBoxLayout *headerLayout = new QHBoxLayout(taskTopPart); // Горизонтальный слой

    // Название проекта
    QLabel *title = new QLabel("Новая задача", this);
    title->setStyleSheet("background-color: transparent; color: black; font-weight: bold; font-size: 25px;");
    headerLayout->addWidget(title);

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
    QLabel *description_tsk = new QLabel("Описание задачи");
    description_tsk->setStyleSheet("background-color: transparent; color: black; font-size: 15px;");
    taskLayout->addWidget(description_tsk, 0, Qt::AlignTop | Qt::AlignLeft);

}

TaskWindow::~TaskWindow()
{
}
