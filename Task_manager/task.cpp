#include "task.h"
#include "taskwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QFrame>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>


Task::Task(QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *taskBox = new QLabel(this);
    taskBox->setStyleSheet("background-color: #a7bfa5;");
    taskBox->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->addWidget(taskBox);

    QVBoxLayout *taskBoxLayout = new QVBoxLayout(taskBox);

    // Стиль полей
    QString lineEditStyle = "background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;";

    // Стиль кнопок
    QString buttonStyle = "background-color: #3b4f2a; color: white; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Отступ сверху
    taskBoxLayout->addSpacing(5);

    // Поле ввода названия задачи
    titleEdit = new QLineEdit(this);
    titleEdit->setStyleSheet(lineEditStyle + "font-weight: bold;");
    titleEdit->setFixedHeight(40);
    titleEdit->setPlaceholderText("Название задачи");
    taskBoxLayout->addWidget(titleEdit);

    // Отступ
    taskBoxLayout->addSpacing(5);

    // Поле ввода описания задачи
    descriptionEdit = new QTextEdit(this);
    descriptionEdit->setStyleSheet(lineEditStyle);
    descriptionEdit->setFixedHeight(60);
    descriptionEdit->setPlaceholderText("Описание задачи");
    taskBoxLayout->addWidget(descriptionEdit);

    // Горизонтальный слой для кнопки "Выполнить"
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // Заполнитель слева для выравнивания кнопки вправо

    // Кнопка открытия задачи
    openButton = new QPushButton("Открыть", this);
    openButton->setFixedSize(100, 40);
    openButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(openButton);

    // Кнопка выполнения
    doneButton = new QPushButton("Выполнить", this);
    doneButton->setFixedSize(100, 40);
    doneButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(doneButton);

    // Кнопка сохранения
    saveButton = new QPushButton("Сохранить", this);
    saveButton->setFixedSize(100, 40);
    saveButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(saveButton);

    // Добавляем компоновку кнопок в рамку
    taskBoxLayout->addLayout(buttonLayout);

    // Добавляем рамку в главный слой
    layout->addWidget(taskBox);

    setMinimumSize(400, 200); // Устанавливаем минимальный размер окна

    connect(doneButton, &QPushButton::clicked, this, &Task::markAsDone);
    connect(openButton, &QPushButton::clicked, this, &Task::openTask);
    connect(saveButton, &QPushButton::clicked, this, &Task::saveTask);
}

void Task::markAsDone() {
    // Проверка на пустые поля
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8079"); // Замените на ваш URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для авторизации
    QJsonObject json;
    json["action"] = "destruction";
    json["task_name"] = titleEdit->text(); // Указываем название задачи
    json["task_text"] = descriptionEdit->placeholderText(); // Указываем название задачи
    // Преобразуем JSON объект в документ и выводим его в консоль для отладки
    QJsonDocument jsonDoc(json);

    // Отправляем POST запрос
    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

    // Обрабатываем ответ
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll()).trimmed();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();

            // Проверяем сообщение от сервера
            QString message = jsonObject["message"].toString();
            if (message == "Destruction successful!") {
                QMessageBox::information(this, "Задача удалена", message);
            } else {
                QMessageBox::warning(this, "Ошибка при удалении задачи", message);
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера: " + reply->errorString());
        }
        reply->deleteLater(); // Освобождаем память
        this->deleteLater();
    });

    // Обрабатываем ошибки сети
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply]() {
        QMessageBox::warning(this, "Ошибка", "Ошибка сети: " + reply->errorString());
        this->deleteLater();
    });
    TaskWindow *taskWindow = new TaskWindow(this);

    taskWindow->show(); // Удаляем задачу
}

void Task::openTask() {

    TaskWindow *taskWindow = new TaskWindow(this);

    taskWindow->show();
}

void Task::saveTask() {
    // Проверка на пустые поля
    if (titleEdit->text().isEmpty() || descriptionEdit->placeholderText().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Название и описание не могут быть пустыми");
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8079"); // Замените на ваш URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для авторизации
    QJsonObject json;
    json["action"] = "creation";
    json["task_name"] = titleEdit->text(); // Указываем название задачи
    json["task_text"] = descriptionEdit->placeholderText(); // Указываем название задачи

    // Преобразуем JSON объект в документ и выводим его в консоль для отладки
    QJsonDocument jsonDoc(json);

    // Отправляем POST запрос
    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

    // Обрабатываем ответ
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll()).trimmed();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();

            // Проверяем сообщение от сервера
            QString message = jsonObject["message"].toString();
            if (message == "Task creation successful!") {
                QMessageBox::information(this, "Задача сохранена", message);
            } else {
                QMessageBox::warning(this, "Ошибка при сохранении задачи", message);
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера: " + reply->errorString());
        }
        reply->deleteLater(); // Освобождаем память
    });

    // Обрабатываем ошибки сети
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply]() {
        QMessageBox::warning(this, "Ошибка", "Ошибка сети: " + reply->errorString());
    });
}

