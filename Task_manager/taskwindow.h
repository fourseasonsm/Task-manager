#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QDialog>

class TaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TaskWindow(QWidget *parent = nullptr);
    ~TaskWindow();

private:

};

#endif // TASKWINDOW_H
