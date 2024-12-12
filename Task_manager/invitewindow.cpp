#include "invitewindow.h"
#include "project.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>
#include <QDialog>
#include <QMessageBox>


InviteWindow::InviteWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Приглашение");
    resize(200, 160);

    // Установка фона для всего окна
    setStyleSheet("background-color: #a7bfa5;");

    // Главный компоновщик
    QVBoxLayout *inviteLayout = new QVBoxLayout(this);

    // Отступ сверху
    inviteLayout->addSpacing(10);

    // Заголовок
    QLabel *inviteTitle = new QLabel("Введите логин пользователя, которого хотите пригласить");
    inviteTitle->setStyleSheet("color: black; font-size: 15px;"); // font-family: 'Noto Sans' - заеб какой-то пока со шрифтами
    inviteLayout->addWidget(inviteTitle, 0, Qt::AlignTop | Qt::AlignCenter);

    // Отступ
    inviteLayout->addSpacing(5);

    // Стиль полей и кнопой
    QString lineEditStyle = "background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;";
    QString buttonStyle = "background-color: #3b4f2a; color: white; outline: none; border: none; border-radius: 5px; padding: 10px;";


    // Поле логина
    inviteLineEdit = new QLineEdit(this);
    inviteLineEdit->setFixedSize(300, 30);
    inviteLineEdit->setStyleSheet(lineEditStyle);
    inviteLineEdit->setPlaceholderText("Введите логин пользователя"); // Текст-заполнитель
    inviteLayout->addWidget(inviteLineEdit, 0, Qt::AlignCenter);

    // Отступ
    inviteLayout->addSpacing(10);

    // Кнопка "Пригласить"
    QPushButton *inviteButtonInWindow = new QPushButton("Сохранить адресата", this);
    inviteButtonInWindow->setStyleSheet(buttonStyle);
    inviteButtonInWindow->setFixedSize(150, 35);

    inviteLayout->addWidget(inviteButtonInWindow, 0, Qt::AlignCenter);
    connect(inviteButtonInWindow, &QPushButton::clicked, this, &InviteWindow::on_inviteButtonInWindow_clicked);

    // Отступ
    inviteLayout->addSpacing(5);

    // Установка главного компоновщика
    setLayout(inviteLayout);
}

InviteWindow::~InviteWindow() {
}

void InviteWindow::on_inviteButtonInWindow_clicked() {
    emit invite_user(inviteLineEdit->text()); // Написать, что происходит после нажатия кнопки "Пригласить" в окне
    QMessageBox::information(this, "Адресат сохранен","Адресат сохранен");
}
