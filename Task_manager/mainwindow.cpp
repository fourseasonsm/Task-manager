#include "mainwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>


// Цвета: Средний зеленый -  #a7bfa5, светлый зеленый - #e1f0db, темный зеленый - #3b4f2a


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Main Window");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Белая шапка
    QLabel *topPart = new QLabel();
    topPart->setStyleSheet("background-color: white;");
    topPart->setFixedHeight(50);
    topPart->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(topPart);

    // Разделитель
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setLineWidth(2);
    separator->setStyleSheet("color: #3b4f2a;");
    mainLayout->addWidget(separator);

    QHBoxLayout *bottomLayout = new QHBoxLayout(); // Горизонтальный слой

    // Зеленая полоса слева
    QLabel *bottomLeftStripe = new QLabel(this);
    bottomLeftStripe->setStyleSheet("background-color: #a7bfa5;");
    bottomLeftStripe->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    bottomLayout->addWidget(bottomLeftStripe);

    QVBoxLayout *leftStripeLayout = new QVBoxLayout(bottomLeftStripe); // Вертикальный слой для левой полосы

    // Текст "Профиль"
    QLabel *overlay_prf = new QLabel("Профиль", bottomLeftStripe);
    overlay_prf->setStyleSheet("background-color: transparent; color: black; font-weight: bold; font-size: 19px;");
    leftStripeLayout->addWidget(overlay_prf, 0, Qt::AlignTop | Qt::AlignHCenter);


    // Белая центральная полоса с прокруткой
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // Автоматическая подстройка размера
    scrollArea->setStyleSheet("background-color: #e1f0db;");

    // Внутренний виджет для области прокрутки
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *centerStripeLayout = new QVBoxLayout(scrollContent); // Вертикальный слой для центральной полосы


    // Текст "Текущие задачи"
    QLabel *overlay_tsk = new QLabel("Текущие задачи", scrollContent);
    overlay_tsk->setStyleSheet("background-color: transparent; color: black; font-weight: bold; font-size: 19px;");
    centerStripeLayout->addWidget(overlay_tsk, 0, Qt::AlignTop | Qt::AlignLeft);

    // Контейнер для задач
    tasksLayout = new QVBoxLayout(); // Создаем новый вертикальный слой для задач
    centerStripeLayout->addLayout(tasksLayout); // Добавляем контейнер задач

    // Кнопка создания новой задачи
    QPushButton *newTaskButton = new QPushButton("+", this);
    newTaskButton->setFixedSize(40, 40);
    newTaskButton->setStyleSheet("background-color: #3b4f2a; color: white; font-size: 18px; border-radius: 20px;");
    centerStripeLayout->addWidget(newTaskButton, 0, Qt::AlignBottom | Qt::AlignRight);

    // Добавляем контент в QScrollArea
    scrollArea->setWidget(scrollContent);
    bottomLayout->addWidget(scrollArea);

    // Подключаем сигнал нажатия кнопки к слоту добавления новой задачи
    connect(newTaskButton, &QPushButton::clicked, this, &MainWindow::createNewTask);


    // Зеленая полоса справа
    QLabel *bottomRightStripe = new QLabel(this);
    bottomRightStripe->setStyleSheet("background-color: #a7bfa5;");
    bottomRightStripe->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    bottomLayout->addWidget(bottomRightStripe);

    QVBoxLayout *rightStripeLayout = new QVBoxLayout(bottomRightStripe); // Вертикальный слой для правой полосы

    // Текст "Пользователи онлайн"
    QLabel *overlay_usr = new QLabel("Пользователи онлайн", bottomRightStripe);
    overlay_usr->setStyleSheet("background-color: transparent; color: black; font-weight: bold; font-size: 19px;");
    rightStripeLayout->addWidget(overlay_usr, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Устанавливаем размеры полос
    bottomLayout->setStretch(0, 2); // Зелёная полоса слева
    bottomLayout->setStretch(1, 5); // Белая центральная полоса
    bottomLayout->setStretch(2, 2); // Зелёная полоса справа
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout); // Устанавливаем главный слой для окна
}


// Слот для создания новой задачи
void MainWindow::createNewTask() {
    Task *newTask = new Task("Новая задача", "Описание задачи", this);
    tasksLayout->addWidget(newTask); // Добавляем новую задачу в tasksLayout
}


