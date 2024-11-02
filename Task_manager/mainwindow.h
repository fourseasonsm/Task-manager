// #ifndef MAINWINDOW_HPP
// #define MAINWINDOW_HPP

// #include "task.h"

// #include <QApplication>
// #include <QWidget>
// #include <QLabel>
// #include <QVBoxLayout>
// #include <QPushButton>
// #include <QScrollArea>

// class MainWindow : public QWidget {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);

// private slots:
//     void createNewTask(); // Объявление слота для создания новой задачи

// private:
//     QScrollArea *scrollArea; // Указатель на QScrollArea
//     QVBoxLayout *taskLayout; // Layout для хранения задач
// };

// #endif // MAINWINDOW_HPP



#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "task.h"

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void createNewTask(); // Объявление слота для создания новой задачи

private:
    QScrollArea *scrollArea; // Указатель на QScrollArea
    QVBoxLayout *tasksLayout; // Layout для хранения задач
    QVBoxLayout *centerStripeLayout; // Вертикальный слой для центральной полосы
};

#endif // MAINWINDOW_HPP
