#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QString>

class TaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TaskWindow(QWidget *parent = nullptr,const QString task_name = "Пусто ",const QString task_text = "Ничего нет");
    ~TaskWindow();
signals:
    void savetask ();// Сигнал для сохранения подзадачи
    void donetask ();// Сигнал для закрытия подзадачи
    void Change_task_info (const QString &task_name,const QString &task_text);// Сигнал для изменения названия задачи
private slots:
    void on_saveButton_clicked();
    void on_doneButton_clicked();
    void on_closeButton_clicked();

private:
    QLineEdit *title; // Поле для редактирования названия
    QTextEdit *description_tsk; // Поле для редактирования описания
};

#endif // TASKWINDOW_H
