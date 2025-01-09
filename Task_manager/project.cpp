#include "project.h"
#include "projectwindow.h"
#include "mainwindow.h"
#include "invitewindow.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QScrollArea>
#include <QTextEdit>
#include <QIntValidator>
#include <QFrame>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

Project::Project(const QString &smallServerUrl, QWidget *parent)
    : QWidget(parent), smallServerUrl(smallServerUrl){
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *projectBox = new QLabel(this);
    projectBox->setStyleSheet("background-color: #a7bfa5;");
    projectBox->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    layout->addWidget(projectBox);

    QVBoxLayout *projectBoxLayout = new QVBoxLayout(projectBox); // Вертикальный слой внутри рамки

    // Стиль полей
    QString lineEditStyle = "background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;";

    // Стиль кнопок
    QString buttonStyle = "background-color: #3b4f2a; color: white; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Отступ сверху
    projectBoxLayout->addSpacing(10);

    // Поле ввода названия проекта
    titleEdit = new QLineEdit(this);
    titleEdit->setStyleSheet(lineEditStyle + "font-weight: bold;");
    titleEdit->setFixedHeight(30);
    titleEdit->setPlaceholderText("Название проекта");
    projectBoxLayout->addWidget(titleEdit);

    // Отступ
    projectBoxLayout->addSpacing(10);

    // Поле ввода описания проекта
    descriptionEdit = new QTextEdit(this);
    descriptionEdit->setStyleSheet(lineEditStyle);
    descriptionEdit->setFixedHeight(70);
    descriptionEdit->setPlaceholderText("Описание проекта");
    projectBoxLayout->addWidget(descriptionEdit);

    // Текст "Подзадачи проекта"
    QLabel *sub_task = new QLabel("Подзадачи проекта", this);
    sub_task->setStyleSheet("color: black; font-size: 16px;");
    projectBoxLayout->addWidget(sub_task);

    // Создаем контейнер для подзадач
    QWidget *subTasksContainer = new QWidget(this);
    subTasksLayout = new QVBoxLayout(subTasksContainer);  // Вертикальная компоновка для подзадач
    subTasksContainer->setLayout(subTasksLayout);

    // Оборачиваем контейнер для подзадач в QScrollArea
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(subTasksContainer);  // Устанавливаем контейнер с подзадачами в QScrollArea
    scrollArea->setWidgetResizable(true);  // Устанавливаем, чтобы содержимое растягивалось
    projectBoxLayout->addWidget(scrollArea);

    // Горизонтальный слой для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Кнопка добавления подзадачи
    subTaskButton = new QPushButton("Добавить задачу", this);
    subTaskButton->setFixedSize(130, 40);
    subTaskButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(subTaskButton);

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

    // Кнопка закрытия окна (Рената может её облагородить)
    closeButton = new QPushButton("✖", this); // Используем символ крестика
    closeButton->setFixedSize(40, 40);
    closeButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(closeButton);

    // Добавляем компоновку кнопок в рамку
    projectBoxLayout->addLayout(buttonLayout);

    // Добавляем рамку в главный слой
    layout->addWidget(projectBox);

    setMinimumSize(400, 350); // Устанавливаем минимальный размер окна

    // Разрешение изменения размера окна
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(openButton, &QPushButton::clicked, this, &Project::openProject);
    connect(subTaskButton, &QPushButton::clicked, this, &Project::addSubTask);
    connect(doneButton, &QPushButton::clicked, this, &Project::markAsDone);
    connect(saveButton, &QPushButton::clicked, this, &Project::saveProject);
    connect(closeButton, &QPushButton::clicked, this, &Project::close);
}

void Project::markAsDone() {
    // Проверка, что проект уже сохранен и имеет ID
    if (project_id.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Проект еще не сохранен на сервере.");
        return;
    }

    // Создаем QNetworkAccessManager для отправки запроса
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QUrl url(URL); 
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для удаления проекта
    QJsonObject json;
    json["action"] = "delete_project";
    json["login"] = user_login_global; // Логин пользователя
    json["project_id"] = project_id;  // ID проекта

    // Преобразуем JSON объект в документ и выводим его в консоль для отладки
    QJsonDocument jsonDoc(json);
    qDebug() << jsonDoc;

    // Отправляем POST запрос
    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

    // Обрабатываем ответ
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll()).trimmed();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();

            // Проверяем сообщение от сервера
            if (jsonObject.contains("message")) {
                QString message = jsonObject["message"].toString();
                if (message == "Копия проекта выполнена и удалена" || message == "Оригинальный проект полностью выполнен и успешно удален") {
                    QMessageBox::information(this, "Проект завершен", message);
                    this->deleteLater(); // Удаляем объект Project после успешного удаления проекта
                } else {
                    QMessageBox::warning(this, "Ошибка при завершении проекта", message);
                }
            } else if (jsonObject.contains("error")) {
                QString errorMessage = jsonObject["error"].toString();
                QMessageBox::warning(this, "Ошибка", "Ошибка сервера: " + errorMessage);
            } else {
                QMessageBox::warning(this, "Ошибка", "Неизвестный ответ от сервера");
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера: " + reply->errorString());
        }
        reply->deleteLater(); // Освобождаем память
    });

    // Обрабатываем ошибки сети
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply]() {
        QMessageBox::warning(this, "Ошибка", "Ошибка сети: " + reply->errorString());
        reply->deleteLater(); // Освобождаем память
    });
}

void Project::openProject() {

    ProjectWindow *projectWindow = new ProjectWindow(this);
    connect(projectWindow, SIGNAL(addtask()), this, SLOT(addSubTask()));

    projectWindow->show();
}

/* void Project::saveProject() {
    QString temp = extractSubTasksInfo();
    // Проверка на пустые поля
    if (titleEdit->text().isEmpty() || descriptionEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Название и описание не могут быть пустыми");
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(URL); // Замените на ваш URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для авторизации
    QJsonObject json;
    json["action"] = "save_project";
    json["login"] = user_login_global;
    json["project_name"] = titleEdit->text(); // Указываем название задачи
    json["project_text"] = descriptionEdit->toPlainText(); // Указываем название задачи
    json["subtasks"] =temp; // Указываем название задачи

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
            QString project_id_temp = jsonObject["project_id"].toString();
            QMessageBox::information(this, "Проект сохранен", message);
            if (message == "Project creation successful!") {
                project_id=project_id_temp;
                QMessageBox::information(this, "Проект сохранен", message);
            } else {
                QMessageBox::warning(this, "Ошибка при сохранении проекта", message);
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
*/

    void Project::saveProject() {
        QString temp = extractSubTasksInfo();
        // Проверка на пустые поля
        if (titleEdit->text().isEmpty() || descriptionEdit->toPlainText().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Название и описание не могут быть пустыми");
            return;
        }

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        QUrl url(smallServerUrl);

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // Создаем JSON объект с данными для авторизации
        QJsonObject json;
        json["action"] = "save_project";
        json["login"] = user_login_global;
        json["project_name"] = titleEdit->text(); // Указываем название задачи
        json["project_text"] = descriptionEdit->toPlainText(); // Указываем название задачи
        json["subtasks"] = temp; // Указываем название задачи

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
                QString project_id_temp = jsonObject["project_id"].toString();
                QMessageBox::information(this, "Проект сохранен", project_id_temp);
                if (message == "Project creation successful!") {
                    project_id=project_id_temp; // Отправляем сигнал с project_id
                    QMessageBox::information(this, "Проект сохранен", message);
                } else {
                    QMessageBox::warning(this, "Ошибка при сохранении проекта", message);
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


void Project::addSubTask() {
    // Создаём горизонтальный слой для подзадачи
    QHBoxLayout *subTaskLayout = new QHBoxLayout();

    // Создаём новое поле ввода для подзадачи
    QLineEdit *newSubTask = new QLineEdit(this);
    newSubTask->setPlaceholderText("Подзадача");
    newSubTask->setFixedHeight(22);
    newSubTask->setStyleSheet("background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;");

    // Поле для ввода веса подзадачи
    QLineEdit *subTaskWeight = new QLineEdit(this);
    subTaskWeight->setValidator(new QIntValidator(1, 10, this));
    subTaskWeight->setPlaceholderText("1-10");
    subTaskWeight->setFixedSize(25, 22);
    subTaskWeight->setAlignment(Qt::AlignCenter);
    subTaskWeight->setStyleSheet("background-color: #e1f0db; color: black; font-size: 10px; outline: none; border: none;");

    // Поле для ввода назначенного пользователя
    QLineEdit *assignedUser = new QLineEdit(this);
    assignedUser->setPlaceholderText("Кому назначено");
    assignedUser->setFixedHeight(22);
    assignedUser->setStyleSheet("background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;");

    // Создаём кнопку "Пригласить"
    QPushButton *inviteButton = new QPushButton("Пригласить", this);
    inviteButton->setStyleSheet("background-color: #3b4f2a; color: white; font-size: 12px; outline: none; border: none; border-radius: 5px; padding: 3px 8px;");
    inviteButton->setFixedHeight(22);

    // Создаём метку для отображения приглашённого пользователя
    QLabel *invitedUserLabel = new QLabel(this);
    invitedUserLabel->setStyleSheet("color: black; font-size: 12px;");

    // Добавляем все элементы в горизонтальный слой
    subTaskLayout->addWidget(newSubTask);
    subTaskLayout->addWidget(subTaskWeight);
    subTaskLayout->addWidget(assignedUser); // Добавляем поле для назначения
    subTaskLayout->addWidget(inviteButton);
    subTaskLayout->addWidget(invitedUserLabel);

    // Добавляем подзадачу в контейнер
    subTasksLayout->addLayout(subTaskLayout);

    // Обновляем отображение
    updateGeometry();

    // Подключаем сигнал измененния текста к слоту
    connect(subTaskWeight, &QLineEdit::textChanged, this, &Project::textChanged);

    // Подключаем сигнал кнопки "Пригласить" к слоту
    // Подключаем сигнал кнопки "Пригласить" к слоту
       connect(inviteButton, &QPushButton::clicked, [this, newSubTask, invitedUserLabel,assignedUser,subTaskWeight](){
       QNetworkAccessManager *manager = new QNetworkAccessManager(this);

       QUrl url(smallServerUrl); 

       QNetworkRequest request(url);
       request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

       // Создаем JSON объект с данными для авторизации
       QJsonObject json;
       json["action"] = "send_invitation";
       json["target_login"] = assignedUser->text();
       json["project_text"] = descriptionEdit->toPlainText();
       json["project_name"] = titleEdit->text();
       json["subtask_text"] = newSubTask->text();
       json["subtask_weight"] = subTaskWeight->text();
       json["project_id"] = project_id;
       json["login"] = user_login_global;


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
               if (message == "Subtask") {
                   QMessageBox::information(this, "Задача сохранена", "Задача успешно сохранена");
               } else {
                   QMessageBox::warning(this, "Ошибка при сохранении задачи", "Задача не сохранена");
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
       });
}

void Project::invite_user(const QString &invited_user_login){
    assignedUser->setText(invited_user_login);
}
void Project::on_invite_clicked(){
    InviteWindow *inviteWindow = new InviteWindow(this);
    connect(inviteWindow, SIGNAL(invite_user(const QString)), this, SLOT(invite_user(QString)));
    inviteWindow->show();
}
QString Project::extractSubTasksInfo() {
    // List to store sub-task information
    QString subTasksInfo;

    // Check if subTasksLayout is initialized
    if (!subTasksLayout) {
        qDebug() << "subTasksLayout is not initialized!";
        return subTasksInfo; // Return empty list
    }

    // Iterate through the subTasksLayout
    for (int i = 0; i < subTasksLayout->count(); ++i) {
        QLayoutItem *item = subTasksLayout->itemAt(i);

        // Check if the item is a layout (QHBoxLayout)
        if (QHBoxLayout *subTaskLayout = qobject_cast<QHBoxLayout *>(item->layout())) {
            QString taskName;
            int taskWeight = 0;
            QString assignedUser;

            // Iterate through the widgets in the QHBoxLayout
            for (int j = 0; j < subTaskLayout->count(); ++j) {
                QLayoutItem *subItem = subTaskLayout->itemAt(j);

                // Check if the item is a widget
                if (QWidget *widget = subItem->widget()) {
                    // If it's the QLineEdit for the task name
                    if (QLineEdit *taskNameEdit = qobject_cast<QLineEdit *>(widget)) {
                        taskName = taskNameEdit->text(); // Get the task name
                        subTasksInfo=subTasksInfo+taskName+',';
                    }
                }
            // Store the task information if valid
            }
        }
    subTasksInfo = subTasksInfo + ';'; // Debug output
    }
    return subTasksInfo; // Return the list of sub-task information
}

void Project::setTitle(const QString& title) {
    titleEdit->setText(title);
}

void Project::setDescription(const QString& description) {
    descriptionEdit->setText(description);
}

void Project::addSubTaskFromServer(int subtask_id, const QString& subtask_text, int subtask_weight, const QString& status, int number, QString username_local) {
    QHBoxLayout *subTaskLayout = new QHBoxLayout();

    // Поле для названия подзадачи
    QLineEdit *newSubTask = new QLineEdit(this);
    newSubTask->setText(subtask_text);
    newSubTask->setFixedHeight(22);
    newSubTask->setStyleSheet("background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;");

    QLineEdit *assignedUsr = new QLineEdit(this);
    assignedUsr->setText(username_local);
    assignedUsr->setFixedHeight(22);
    assignedUsr->setStyleSheet("background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;");


    // Поле для веса подзадачи
    QLineEdit *subTaskWeight = new QLineEdit(this);
    subTaskWeight->setText(QString::number(subtask_weight));
    subTaskWeight->setValidator(new QIntValidator(1, 10, this));
    subTaskWeight->setFixedSize(25, 22);
    subTaskWeight->setAlignment(Qt::AlignCenter);
    subTaskWeight->setStyleSheet("background-color: #e1f0db; color: black; font-size: 10px; outline: none; border: none;");

    QPushButton *inviteButton = new QPushButton("Пригласить", this);
       inviteButton->setStyleSheet("background-color: #3b4f2a; color: white; font-size: 12px; outline: none; border: none; border-radius: 5px; padding: 3px 8px;");
       inviteButton->setFixedHeight(22);

    // Поле для статуса подзадачи
    QLabel *statusLabel = new QLabel(status, this);
    statusLabel->setStyleSheet("color: black; font-size: 12px;");

    // Добавляем все элементы в горизонтальный слой
    subTaskLayout->addWidget(newSubTask);
    subTaskLayout->addWidget(subTaskWeight);
    subTaskLayout->addWidget(statusLabel);
    subTaskLayout->addWidget(inviteButton);
    subTaskLayout->addWidget(assignedUsr);

    // Добавляем подзадачу в контейнер
    subTasksLayout->addLayout(subTaskLayout);
    connect(inviteButton, &QPushButton::clicked, [this, newSubTask, subTaskWeight,assignedUsr](){
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);

            QUrl url(smallServerUrl); 

            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            // Создаем JSON объект с данными для авторизации
            QJsonObject json;
            json["action"] = "send_invitation";
            json["target_login"] = assignedUsr->text();
            json["project_text"] = descriptionEdit->toPlainText();
            json["project_name"] = titleEdit->text();
            json["subtask_text"] = newSubTask->text();
            json["subtask_weight"] = subTaskWeight->text();
            json["project_id"] = project_id;
            json["login"] = user_login_global;

            // Преобразуем JSON объект в документ и выводим его в консоль для отладки
            QJsonDocument jsonDoc(json);

            // Отправляем POST запрос
            QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

            // Обрабатываем ответ
            QMessageBox::information(this, "Задача сохранена", "Задача успешно сохранена");
            connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QString response = QString::fromUtf8(reply->readAll()).trimmed();
                    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
                    QJsonObject jsonObject = jsonResponse.object();

                    // Проверяем сообщение от сервера
                    QString message = jsonObject["message"].toString();
                    if (message == "Подзадача успешно отправлена") {
                        QMessageBox::information(this, "Задача сохранена", "Задача успешно сохранена");
                    } else {
                        QMessageBox::warning(this, "Ошибка при сохранении задачи", "Задача не сохранена");
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
        });
}

void Project::textChanged() {
    QLineEdit *subTaskWeight = qobject_cast<QLineEdit*>(sender());
    if (!subTaskWeight) return;

    bool ok;
    int value = subTaskWeight->text().toInt(&ok);

    if (ok && value >= 1 && value <= 10) {
        QColor color;

        if (value == 1) {
            color = QColor(90, 235, 71);  // Зеленый для 1
        } else if (value == 2) {
            color = QColor(161, 235, 71);  // для 2
        } else if (value == 3) {
            color = QColor(221, 235, 71);  // для 3
        } else if (value == 4) {
            color = QColor(235, 191, 71);  // для 4
        } else if (value == 5) {
            color = QColor(235, 167, 71);  // для 5
        } else if (value == 6) {
            color = QColor(235, 145, 71);  // для 6
        } else if (value == 7) {
            color = QColor(235, 126, 71);  // для 7
        } else if (value == 8) {
            color = QColor(235, 107, 71);  // для 8
        } else if (value == 9) {
            color = QColor(235, 88, 71);  // для 9
        } else if (value == 10) {
            color = QColor(235, 71, 71);  // для 10
        } else {
            color = QColor(255, 255, 255);  // Белый для остальных значений
        }

        // Применяем цвет фона
        subTaskWeight->setStyleSheet(QString("background-color: %1; color: black; font-size: 10px; outline: none; border: none;").arg(color.name()));
    } else {
        subTaskWeight->setStyleSheet("background-color: #e1f0db; color: black; font-size: 10px; outline: none; border: none;");  // Если введено неверное число, оставляем белый фон
    }
}

void Project::close() {

   QWidget::close(); // Вызываем метод close базового класса
}
