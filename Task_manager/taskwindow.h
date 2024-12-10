#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>

class TaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TaskWindow(QWidget *parent = nullptr);
    ~TaskWindow();

private slots:
    void on_saveButton_clicked();
    void on_doneButton_clicked();
    void on_closeButton_clicked();

private:
    QLineEdit *title; // Поле для редактирования названия
    QTextEdit *description_tsk; // Поле для редактирования описания
};

#endif // TASKWINDOW_H
