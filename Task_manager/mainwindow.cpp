#include "mainwindow.h"
#include "loginwindow.h"
#include "global.h"
#include "registrationwindow.h"
#include "projectwindow.h"
#include "task.h"
#include "taskwindow.h"
<<<<<<< HEAD

=======
#include "global.h"
>>>>>>> cc19da91e259b3fd21b93732cb41b101ad3d0d9e
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QFrame>
<<<<<<< HEAD
#include <QGridLayout>
=======
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
>>>>>>> cc19da91e259b3fd21b93732cb41b101ad3d0d9e

bool isLoggedIn = false;

// Цвета: Средний зеленый -  #a7bfa5, светлый зеленый - #e1f0db, темный зеленый - #3b4f2a


MainWindow::MainWindow(QWidget *parent) : QWidget(parent), scrollArea(new QScrollArea(this)) {
    setWindowTitle("Task Manager");
    resize(1200, 700);

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
<<<<<<< HEAD
=======
    // addShadowEffect(logoutButton); // Добавляем тень
>>>>>>> cc19da91e259b3fd21b93732cb41b101ad3d0d9e

    headerLayout->addWidget(logoutButton, 0, Qt::AlignRight);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);

    // Кнопка регистрации
    regButton = new QPushButton("Зарегистрироваться", this);
    regButton->setFixedSize(150, 35);
    regButton->setStyleSheet(buttonStyle);

    headerLayout->addWidget(regButton, 0, Qt::AlignRight);
    connect(regButton, &QPushButton::clicked, this, &MainWindow::on_regButton_clicked);    connect(regButton, &QPushButton::clicked, this, &MainWindow::on_regButton_clicked);


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
    QLabel *user_name = new QLabel(user_login_global, bottomLeftStripe);
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
    QLabel *usersBox = new QLabel(bottomRightStripe);
    usersBox->setStyleSheet("background-color: #e1f0db;");
    usersBox->setFixedSize(200, 300);
    rightStripeLayout->addWidget(usersBox, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Слой для пользователей онлайн
    QVBoxLayout *usersBoxLayout = new QVBoxLayout(usersBox);

    // Список пользователей онлайн
    QListWidget *usersList = new QListWidget(usersBox);
    usersList->setStyleSheet("background-color: #e1f0db; color: black; font-size: 18px");
    usersList->setFixedSize(200, 300);

    // Проверка на пустые поля
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8080"); // Замените на ваш URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с данными для авторизации
    QJsonObject json;
    json["action"] = "list_of_user"; // Указываем действие

    // Преобразуем JSON объект в документ и выводим его в консоль для отладки
    QJsonDocument jsonDoc(json);

    // Отправляем POST запрос
    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

    // Обрабатываем ответ
    connect(reply, &QNetworkReply::finished, this, [ usersList, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll()).trimmed();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();

            // Проверяем сообщение от сервера
            QString message = jsonObject["message"].toString();
            QString list_of_user = jsonObject["list_of_users"].toString();
            if (message == "List sended") {
                QString temp = "";
                for (int i =0;i<=list_of_user.size()-1;i++){
                    if(list_of_user[i] != ','){
                        temp = temp + list_of_user[i];
                    }
                    else {
                        usersList->addItem(temp);
                        temp = "";
                    }
                }
                usersList->addItem(temp);
            } else {
                qDebug()<< message;
            }
        } else {
            qDebug()<< "Ошибка"<< "Не удалось получить ответ от сервера: " << reply->errorString();
        }
        reply->deleteLater(); // Освобождаем память
    });

    // Обрабатываем ошибки сети
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply]() {
        QMessageBox::warning(this, "Ошибка", "Ошибка сети: " + reply->errorString());
    });

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
<<<<<<< HEAD
        Task *newTask = new Task(this);
=======
        Task *newTask = new Task(smallServerUrl, this);
>>>>>>> cc19da91e259b3fd21b93732cb41b101ad3d0d9e

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
    isLoggedIn = true;
    updateAuthButtons();

    LoginWindow *loginWindow = new LoginWindow(this);
<<<<<<< HEAD
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);  // Автоматическое удаление окна при закрытии
    loginWindow->show();

=======
    //по сути то же самое, что show, только с show геттер не работает
    if (loginWindow->exec() == QDialog::Accepted) {
        smallServerUrl = loginWindow->getSmallServerUrl();
    }
    updateAuthButtons();
    Load_list_of_tasks();
}

// Нажатие на кнопку выхода
void MainWindow::on_logoutButton_clicked()
{
    isLoggedIn = false;
    updateAuthButtons();
}

void MainWindow::Load_list_of_tasks()
{
    if (isLoggedIn) {
        // Проверка на пустые поля
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QUrl url(smallServerUrl); // Замените на ваш URL
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // Создаем JSON объект с данными для авторизации
        QJsonObject json;
        json["action"] = "list_of_tasks";
        json["login"] = user_login_global; // Замените на ваше поле логина
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
                QString list_of_tasks = jsonObject["list_of_tasks"].toString();
                int counter = 0;
                if (message == "List sended") {
                    QString temp = "";
                    QString task_name_temp;
                    for (int i =0;i<=list_of_tasks.size()-1;i++){
                        if(list_of_tasks[i] != ','){
                            temp = temp + list_of_tasks[i];
                        }
                        else {
                            if (counter % 2 == 0){
                                task_name_temp = temp;
                                temp = "";
                            }
                            else {
                                Task *newTask = new Task(smallServerUrl, this,task_name_temp,temp);
                                tasksLayout->addWidget(newTask);
                                temp = "";
                            }
                            counter++;
                        }
                    }
                    QMessageBox::information(this, "Загрузка задач", "Загрузка задач прошла успешно");
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
>>>>>>> cc19da91e259b3fd21b93732cb41b101ad3d0d9e
}

// Нажатие на кнопку для перехода к окну регистрации
void MainWindow::on_regButton_clicked()
{
    RegistrationWindow *registerWindow = new RegistrationWindow(this);
    registerWindow->show();
}

// Нажатие на кнопку выхода
void MainWindow::on_logoutButton_clicked()
{
    isLoggedIn = false;
    updateAuthButtons();
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
