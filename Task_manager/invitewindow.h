#ifndef INVITEWINDOW_H
#define INVITEWINDOW_H

#include "project.h"

#include <QWidget>
#include <QDialog>
#include <QLineEdit>

class InviteWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InviteWindow(QWidget *parent = nullptr);
    ~InviteWindow();
signals:
    void invite_user (const QString &invited_user_login);// Сигнал для приглашения юзера
private slots:
    void on_inviteButtonInWindow_clicked();

private:
    QLineEdit *inviteLineEdit;
};

#endif // INVITEWINDOW_H
