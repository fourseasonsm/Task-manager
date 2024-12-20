#include "mainwindow.h"
#include "loginwindow.h"
#include "global.h"
#include "registrationwindow.h"
#include "projectwindow.h"
#include "task.h"
#include "taskwindow.h"
#include "project.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


bool isLoggedIn = false;
QString user_login_global="NULL";
// Цвета: Средний зеленый -  #a7bfa5, светлый зеленый - #e1f0db, темный зеленый - #3b4f2a

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), scrollArea(new QScrollArea(this)) {
    setWindowTitle("Task Manager");
    resize(1200, 700);

    mainServerUrls.append("http://localhost:8079");  // Резервный сервер 1
    mainServerUrls.append("http://localhost:8078");  // Резервный сервер 2

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Белая шапка
    QLabel *topPart = new QLabel("Task Manager");
    topPart->setStyleSheet("background-color: white; color: black; font-weight: bold; font-size: 30px;");
    topPart->setFixedHeight(60);
    topPart->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(topPart);

    QHBoxLayout *headerLayout = new QHBoxLayout(topPart); // Горизонтальный слой

    // Стиль кнопок
    QString buttonStyle = "background-color: #3b4f2a; color: white; font-size: 13px; font-weight: bold; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Кнопка создания задачи
    QPushButton *newTaskButton = new QPushButton("Создать задачу", this);
    newTaskButton->setFixedSize(120, 35);
    newTaskButton->setStyleSheet(buttonStyle);

    headerLayout->addWidget(newTaskButton, 0, Qt::AlignLeft);
    connect(newTaskButton, &QPushButton::clicked, this, &MainWindow::createNewTask);

    // Кнопка создания проекта
    QPushButton *newProjectButton = new QPushButton("Создать проект", this);
    newProjectButton->setFixedSize(120, 35);
    newProjectButton->setStyleSheet(buttonStyle);

    headerLayout->addWidget(newProjectButton, 0, Qt::AlignLeft);
    connect(newProjectButton, &QPushButton::clicked, this, &MainWindow::createNewProject);

    // Добавляем пустое пространство слева для выравнивания
    headerLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Кнопка входа
    QPushButton *refreshButton = new QPushButton("Обновить", this);
    refreshButton->setFixedSize(100, 35);
    refreshButton->setStyleSheet(buttonStyle);

    headerLayout->addWidget(refreshButton, 0, Qt::AlignRight);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::on_refreshButton_clicked);

    // Кнопка входа
    authLoginButton = new QPushButton("Войти", this);
    authLoginButton->setFixedSize(100, 35);
    authLoginButton->setStyleSheet(buttonStyle);

    headerLayout->addWidget(authLoginButton, 0, Qt::AlignRight);
    connect(authLoginButton, &QPushButton::clicked, this, &MainWindow::on_authLoginButton_clicked);

    // Кнопка выхода
    logoutButton = new QPushButton("Выйти", this);
    logoutButton->setFixedSize(100, 35);
    logoutButton->setStyleSheet(buttonStyle);
    logoutButton->hide(); // Скрываем кнопку выхода
    // addShadowEffect(logoutButton); // Добавляем тень

    headerLayout->addWidget(logoutButton, 0, Qt::AlignRight);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);

    // Кнопка регистрации
    regButton = new QPushButton("Зарегистрироваться", this);
    regButton->setFixedSize(150, 35);
    regButton->setStyleSheet(buttonStyle);

    headerLayout->addWidget(regButton, 0, Qt::AlignRight);
    connect(regButton, &QPushButton::clicked, this, &MainWindow::on_regButton_clicked);


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

    // Отступ
    leftStripeLayout->addSpacing(30);

    // Имя пользователя
    user_name = new QLabel("Вход не был выполнен", bottomLeftStripe);
    user_name->setStyleSheet("color: black; font-size: 20px; text-decoration: underline;");
    leftStripeLayout->addWidget(user_name, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Добавляем пустое пространство, чтобы элементы располагались сверху
    leftStripeLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));




    // Светло-зеленая центральная полоса с прокруткой
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

    // Добавляем контент в QScrollArea
    scrollArea->setWidget(scrollContent);
    bottomLayout->addWidget(scrollArea);

    // Добавляем пустое пространство, чтобы элементы располагались сверху
    centerStripeLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));




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

    // Отступ
    rightStripeLayout->addSpacing(30);

    // Поле для пользователей онлайн
    QWidget *usersBox = new QWidget(bottomRightStripe);
    usersBox->setStyleSheet("background-color: #e1f0db;");
    usersBox->setFixedSize(200, 300);
    rightStripeLayout->addWidget(usersBox, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Слой для пользователей онлайн
    QVBoxLayout *usersBoxLayout = new QVBoxLayout(usersBox);

    // Список пользователей онлайн
    usersList = new QListWidget(usersBox);
    usersList->setStyleSheet("background-color: #e1f0db; color: black; font-size: 18px");
    usersBoxLayout->addWidget(usersList);

    // Проверка на пустые поля
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url = selectAvailableServer(mainServerUrls);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    updateUsersOnline();

    // Добавляем в слой
    usersBoxLayout->addWidget(usersList, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Добавляем пустое пространство, чтобы элементы располагались сверху
    rightStripeLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));




    // Устанавливаем размеры полос
    bottomLayout->setStretch(0, 2); // Зелёная полоса слева
    bottomLayout->setStretch(1, 5); // Белая центральная полоса
    bottomLayout->setStretch(2, 2); // Зелёная полоса справа
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout); // Устанавливаем главный слой для окна
}


// Слот для создания новой задачи
void MainWindow::createNewTask() {
    if (isLoggedIn == true) {
        Task *newTask = new Task(smallServerUrl, this);

        tasksLayout->addWidget(newTask);

        // Центрируем все задачи по верхнему краю контейнера
        tasksLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    } else {
        QMessageBox::warning(this, "Ошибка",  "Для создания задачи необходимо авторизоваться");
    }
}

// Слот для создания нового проекта
void MainWindow::createNewProject() {
    if (isLoggedIn == true) {
        Project *newProject = new Project(this);

        tasksLayout->addWidget(newProject);

        // Центрируем все проекты по верхнему краю контейнера
        tasksLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    } else {
        QMessageBox::warning(this, "Ошибка", "Для создания проекта необходимо авторизоваться");
    }

}

// Нажатие на кнопку для перехода к окну авторизации
void MainWindow::on_authLoginButton_clicked()
{
    QUrl url = selectAvailableServer(mainServerUrls);
    LoginWindow *loginWindow = new LoginWindow(url, this);
    //Error закрывется не окно логина, а вообще все
//    loginWindow->setAttribute(Qt::WA_DeleteOnClose);  // Автоматическое удаление окна при закрытии

    //по сути то же самое, что show, только с show геттер не работает
    if (loginWindow->exec() == QDialog::Accepted) {
        smallServerUrl = loginWindow->getSmallServerUrl();
    }
    if (isLoggedIn) {
        updateUserName(user_login_global); // Обновляем имя пользователя
        updateAuthButtons();
        updateUsersOnline();
        Load_list_of_tasks();
    }

}

void MainWindow::on_refreshButton_clicked(){
    updateUsersOnline();
    Load_list_of_tasks();
    Load_invitations();
}

bool MainWindow::isServerAvailable(const QString &serverUrl) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(serverUrl + "/healthcheck")); // Предположим, сервер имеет маршрут "/healthcheck" для проверки состояния
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    bool available = reply->error() == QNetworkReply::NoError;
    reply->deleteLater();
    return available;
}

QString MainWindow::selectAvailableServer(const QStringList &serverUrls) {
    for (const QString &serverUrl : serverUrls) {
        if (isServerAvailable(serverUrl)) {
            return serverUrl;
        }
    }
    QString url = "http://localhost:8080";
    return url; // Возвращаем пустую строку, если ни один сервер не доступен
}

//Изменяет имя юзера на переданное
void MainWindow::updateUserName(QString &newUserName) {
    QString displayName = isLoggedIn ? newUserName : "Вход не был выполнен"; // Проверяем, авторизован ли пользователь
    user_name->setText(displayName); // Устанавливаем новый текст для QLabel
}

void MainWindow::updateUsersOnline() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url = selectAvailableServer(mainServerUrls);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["action"] = "list_of_users";

    QJsonDocument jsonDoc(json);
    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll()).trimmed();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();

            if (jsonObject.contains("list_of_users")) {
                QJsonArray jsonArray = jsonObject["list_of_users"].toArray();
                usersList->clear();
                foreach (const QJsonValue &value, jsonArray) {
                    if (value.isString()) {
                        QString username = value.toString();
                        usersList->addItem(username);
                    }
                }
            }
        } else {
            qDebug() << "Error" << "Failed to receive server response:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

/* void MainWindow::Load_list_of_tasks()
{
    if (isLoggedIn) {
        QList<Task*> taskWidgets = tasksLayout->findChildren<Task*>();
        for (Task* task : taskWidgets) {
            tasksLayout->removeWidget(task); // Удаляем виджет из макета
            task->deleteLater(); // Удаляем объект Task
        }

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QUrl url(smallServerUrl); // Замените на ваш URL
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // Создаем JSON объект с данными для авторизации
        QJsonObject json;
        json["action"] = "list_of_tasks";
        json["login"] = user_login_global; // Замените на ваше поле логина
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
                if (message == "List sended") {
                    // Получаем список задач
                    QJsonArray listOfTasks = jsonObject["list_of_tasks"].toArray();

                    for (const QJsonValue& taskValue : listOfTasks) {
                        // Каждый элемент массива — это массив из двух элементов
                        QJsonArray taskArray = taskValue.toArray();
                        if (taskArray.size() == 3) {
                            int task_id = taskArray[0].toInt(); // Идентификатор задачи
                            QString taskName = taskArray[1].toString(); // Имя задачи
                            QString taskText = taskArray[2].toString(); // Текст задачи

                            // Создаем объект Task
                            Task *newTask = new Task(smallServerUrl, this, taskName, taskText, task_id);
                            tasksLayout->addWidget(newTask);
                        } else {
                            qWarning() << "Invalid task format:" << taskArray;
                        }
                    }
                    if (listOfTasks.isEmpty()) {
                        QMessageBox::information(this, "Нет задач", "Вы еще не создали задач.");
                    } else {
                    QMessageBox::information(this, "Загрузка задач", "Загрузка задач прошла успешно");}
                } else {
                    QMessageBox::warning(this, "Ошибка при при отправке задач", message);
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

        // Центрируем все задачи по верхнему краю контейнера
        tasksLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    }
}
*/
void MainWindow::Load_invitations() {
    if (isLoggedIn) {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QUrl url(smallServerUrl);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["action"] = "list_of_invitations";
        json["login"] = user_login_global;
        QJsonDocument jsonDoc(json);

        QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                QString response = QString::fromUtf8(reply->readAll()).trimmed();
                QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
                QJsonObject jsonObject = jsonResponse.object();

                QString message = jsonObject["message"].toString();
                if (message == "Invitations sended") {
                    QJsonArray listOfInvitations = jsonObject["list_of_invitations"].toArray();

                    // Проверяем, что массив приглашений не пустой
                    if (!listOfInvitations.isEmpty()) {
                        QDialog *invitationsDialog = new QDialog(this);
                        invitationsDialog->setWindowTitle("Приглашения");
                        invitationsDialog->setMinimumWidth(400);

                        QVBoxLayout *dialogLayout = new QVBoxLayout(invitationsDialog);

                        for (const QJsonValue& invitationValue : listOfInvitations) {
                            QJsonObject invitationObject = invitationValue.toObject();
                            int invitation_id = invitationObject["invitation_id"].toInt();
                            QString projectName = invitationObject["project_name"].toString();
                            QString projectText = invitationObject["project_text"].toString();
                            int foreign_id = invitationObject["foreign_id"].toInt();
                            QString subtask_text = invitationObject["subtask_text"].toString();
                            QString subtask_weight = invitationObject["subtask_weight"].toString();
                            QString owner_login = invitationObject["owner_login"].toString();
                            int subtask_id = invitationObject["subtask_id"].toInt();

                            // Создаем виджет для отображения приглашения
                            QWidget *invitationWidget = new QWidget();
                            QVBoxLayout *invitationLayout = new QVBoxLayout(invitationWidget);

                            // Добавляем информацию о приглашении
                            QLabel *infoLabel = new QLabel(
                                QString("Проект: %1\nОписание: %2\nПодзадача: %3\nВес: %4\nОтправитель: %5")
                                .arg(projectName)
                                .arg(projectText)
                                .arg(subtask_text)
                                .arg(subtask_weight)
                                .arg(owner_login)
                            );
                            invitationLayout->addWidget(infoLabel);

                            // Добавляем кнопки "Принять" и "Отклонить"
                            QHBoxLayout *buttonsLayout = new QHBoxLayout();
                            QPushButton *acceptButton = new QPushButton("Принять");
                            QPushButton *declineButton = new QPushButton("Отклонить");

                            buttonsLayout->addWidget(acceptButton);
                            buttonsLayout->addWidget(declineButton);
                            invitationLayout->addLayout(buttonsLayout);

                            // Обработчики кнопок
                            connect(acceptButton, &QPushButton::clicked, this, [this, invitation_id]() {
                                sendInvitationResponse(invitation_id, "accept");
                            });

                            connect(declineButton, &QPushButton::clicked, this, [this, invitation_id]() {
                                sendInvitationResponse(invitation_id, "decline");
                            });

                            // Добавляем виджет приглашения в диалоговое окно
                            dialogLayout->addWidget(invitationWidget);
                        }

                        // Показываем диалоговое окно
                        invitationsDialog->setLayout(dialogLayout);
                        invitationsDialog->exec();
                    } else {
                        QMessageBox::information(this, "Нет приглашений", "У вас нет новых приглашений.");
                    }
                } else {
                    QMessageBox::warning(this, "Ошибка при получении приглашений", message);
                }
            } else {
                QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера: " + reply->errorString());
            }
            reply->deleteLater();
        });

        connect(reply, &QNetworkReply::errorOccurred, this, [this, reply]() {
            QMessageBox::warning(this, "Ошибка", "Ошибка сети: " + reply->errorString());
        });
    }
}

void MainWindow::sendInvitationResponse(int invitation_id, const QString &response) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url(smallServerUrl); // Замените на ваш URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для ответа
    QJsonObject json;
    json["action"] = response;
    json["invitation_id"] = invitation_id;
    QJsonDocument jsonDoc(json);

    // Отправляем POST запрос
    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

    // Обрабатываем ответ
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll()).trimmed();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();

            QString message = jsonObject["message"].toString();
            QMessageBox::information(this, "Ответ отправлен", message);
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось отправить ответ: " + reply->errorString());
        }
        reply->deleteLater(); // Освобождаем память
    });

    // Обрабатываем ошибки сети
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply]() {
        QMessageBox::warning(this, "Ошибка", "Ошибка сети: " + reply->errorString());
    });
}

void MainWindow::Load_list_of_tasks() {
    if (isLoggedIn) {
        // Очищаем текущие задачи
        QList<Task*> taskWidgets = tasksLayout->findChildren<Task*>();
        for (Task* task : taskWidgets) {
            tasksLayout->removeWidget(task); // Удаляем виджет из макета
            task->deleteLater(); // Удаляем объект Task
        }

        // Очищаем текущие проекты
        QList<Project*> projectWidgets = tasksLayout->findChildren<Project*>();
        for (Project* project : projectWidgets) {
            tasksLayout->removeWidget(project); // Удаляем виджет из макета
            project->deleteLater(); // Удаляем объект Project
        }

        // Отправляем запрос на сервер
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QUrl url(smallServerUrl); // Замените на ваш URL
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // Создаем JSON объект с данными для запроса
        QJsonObject json;
        json["action"] = "list_of_tasks";
        json["login"] = user_login_global; // Замените на ваше поле логина
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
                if (message == "List sended") {
                    // Получаем список задач
                    QJsonArray listOfTasks = jsonObject["list_of_tasks"].toArray();

                    for (const QJsonValue& taskValue : listOfTasks) {
                        // Каждый элемент массива — это массив из трех элементов
                        QJsonArray taskArray = taskValue.toArray();
                        if (taskArray.size() == 3) {
                            int task_id = taskArray[0].toInt(); // Идентификатор задачи
                            QString taskName = taskArray[1].toString(); // Имя задачи
                            QString taskText = taskArray[2].toString(); // Текст задачи

                            // Создаем объект Task
                            Task *newTask = new Task(smallServerUrl, this, taskName, taskText, task_id);
                            tasksLayout->addWidget(newTask);
                        } else {
                            qWarning() << "Invalid task format:" << taskArray;
                        }
                    }

                    // Получаем список проектов
                    QJsonArray listOfProjects = jsonObject["list_of_projects"].toArray();

                    for (const QJsonValue& projectValue : listOfProjects) {
                        // Каждый элемент массива — это массив из четырех элементов
                        QJsonArray projectArray = projectValue.toArray();
                        if (projectArray.size() == 4) {
                            QString project_id = QString::number(projectArray[0].toInt()); // Идентификатор проекта
                            QString projectName = projectArray[1].toString(); // Имя проекта
                            QString projectText = projectArray[2].toString(); // Текст проекта
                            QJsonArray subtasksArray = projectArray[3].toArray(); // Список подзадач

                            // Создаем объект Project
                            Project *newProject = new Project(this);
                            newProject->setTitle(projectName); // Устанавливаем название проекта
                            newProject->setDescription(projectText); // Устанавливаем описание проекта
                            newProject->project_id=project_id;

                            // Добавляем подзадачи
                            for (const QJsonValue& subtaskValue : subtasksArray) {
                                QJsonArray subtaskData = subtaskValue.toArray();
                                if (subtaskData.size() == 6) {
                                    QString username_local = subtaskData[0].toString(); // Текст подзадачи
                                    int subtask_id = subtaskData[1].toInt(); // Идентификатор подзадачи
                                    QString subtask_text = subtaskData[2].toString(); // Текст подзадачи
                                    int subtask_weight = subtaskData[3].toInt(); // Вес подзадачи
                                    QString status = subtaskData[4].toString(); // Статус подзадачи
                                    int number = subtaskData[5].toInt(); // Порядковый номер подзадачи

                                    // Добавляем подзадачу в проект
                                    newProject->addSubTaskFromServer(subtask_id, subtask_text, subtask_weight, status, number,username_local);
                                } else {
                                    qWarning() << "Invalid subtask format:" << subtaskData;
                                }
                            }

                            tasksLayout->addWidget(newProject);
                        } else {
                            qWarning() << "Invalid project format:" << projectArray;
                        }
                    }
                    QJsonArray listOfinvites = jsonObject["list_of_invitations"].toArray();

                    for (const QJsonValue& projectValue : listOfinvites) {
                        // Каждый элемент массива — это массив из четырех элементов
                        QJsonArray invitesArray = projectValue.toArray();
                        if (invitesArray.size() == 8) {
                            int invintation_id = invitesArray[0].toInt(); // Идентификатор проекта
                            QString projectName_ivites = invitesArray[1].toString(); // Имя проекта
                            QString projectText_invites = invitesArray[2].toString(); // Текст проекта
                            int foreign_id = invitesArray[3].toInt(); // Идентификатор проекта
                            QString subtask_text = invitesArray[4].toString(); // Текст проекта
                            QString subtask_weight= invitesArray[5].toString(); // Текст проекта
                            QString ovner_login= invitesArray[6].toString(); // Текст проекта
                            QJsonArray subtasksArray = invitesArray[7].toArray(); // Список подзадач
                            int subtask_id = invitesArray[8].toInt(); // Идентификатор проекта

                            // Создаем объект Project
                            QMessageBox msg = QMessageBox();
                            msg.setIcon(QMessageBox::Information);
                            msg.setWindowTitle("Приглашение");
                            msg.setInformativeText( projectName_ivites + "\n" + projectText_invites  + "\n"+ subtask_text + "\n"+ subtask_weight + "\n" +ovner_login);
                            msg.addButton("Принять",QMessageBox::AcceptRole);
                            msg.addButton("Отклонить",QMessageBox::RejectRole);
                            bool bttt = msg.exec();
                            if (bttt) {
                                QNetworkAccessManager *manager = new QNetworkAccessManager(this);
                                QUrl url(URL); // Замените на ваш URL
                                QNetworkRequest request(url);
                                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                                // Создаем JSON объект с данными для авторизации
                                QJsonObject json;
                                json["action"] = "send_anwser";
                                json["anwser"] = "yes";

                                // Преобразуем JSON объект в документ и выводим его в консоль для отладки
                                QJsonDocument jsonDoc(json);

                                // Отправляем POST запрос
                                QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

                                // Обрабатываем ошибки сети
                                connect(reply, &QNetworkReply::errorOccurred, this, [this, reply]() {
                                    QMessageBox::warning(this, "Ошибка", "Ошибка сети: " + reply->errorString());
                                });
                        }
                            else {
                                QNetworkAccessManager *manager = new QNetworkAccessManager(this);
                                QUrl url(URL); // Замените на ваш URL
                                QNetworkRequest request(url);
                                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                                // Создаем JSON объект с данными для авторизации
                                QJsonObject json;
                                json["action"] = "send_anwser";
                                json["anwser"] = "no";

                                // Преобразуем JSON объект в документ и выводим его в консоль для отладки
                                QJsonDocument jsonDoc(json);

                                // Отправляем POST запрос
                                QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

                                // Обрабатываем ошибки сети
                                connect(reply, &QNetworkReply::errorOccurred, this, [this, reply]() {
                                    QMessageBox::warning(this, "Ошибка", "Ошибка сети: " + reply->errorString());
                                });
                            }
                        }
                                // Создаем объект Project

                    }
                    // Выводим сообщение в зависимости от наличия задач и проектов
                    if (listOfTasks.isEmpty() && listOfProjects.isEmpty()) {
                        QMessageBox::information(this, "Нет задач и проектов", "Вы еще не создали задач и проектов.");
                    } else if (listOfTasks.isEmpty()) {
                        QMessageBox::information(this, "Нет задач", "Вы еще не создали задач.");
                    } else if (listOfProjects.isEmpty()) {
                        QMessageBox::information(this, "Нет проектов", "Вы еще не создали проектов.");
                    } else {
                        QMessageBox::information(this, "Загрузка задач и проектов", "Загрузка задач и проектов прошла успешно");
                    }
                } else {
                    QMessageBox::warning(this, "Ошибка при получении списка", message);
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

        // Центрируем все задачи и проекты по верхнему краю контейнера
        tasksLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    }
}

// Нажатие на кнопку для перехода к окну регистрации
void MainWindow::on_regButton_clicked()
{
    QUrl url = selectAvailableServer(mainServerUrls);
    RegistrationWindow *registerWindow = new RegistrationWindow(url, this);
    if (registerWindow->exec() == QDialog::Accepted) {
    }
}

// Нажатие на кнопку выхода
void MainWindow::on_logoutButton_clicked()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url = selectAvailableServer(mainServerUrls);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для авторизации
    QJsonObject json;
    json["action"] = "logout"; // Указываем действие
    json["login"] = user_login_global;

    // Преобразуем JSON объект в документ и выводим его в консоль для отладки
    QJsonDocument jsonDoc(json);

    // Отправляем POST запрос
    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

    user_login_global = "NULL";
    isLoggedIn = false;
    updateUserName(user_login_global);
    updateAuthButtons();
    updateUsersOnline();
}

// Меняем кнопки "Войти" и "Зарегистрироваться" на "Выйти" и наоборот
void MainWindow::updateAuthButtons()
{
    if (isLoggedIn) {
        authLoginButton->hide();
        regButton->hide();
        logoutButton->show();
    } else {
        authLoginButton->show();
        regButton->show();
        logoutButton->hide();
    }

    this->update();
}

MainWindow::~MainWindow(){
    on_logoutButton_clicked();
}
