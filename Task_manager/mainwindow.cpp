#include "mainwindow.h"
#include "loginwindow.h"
#include "registrationwindow.h"
#include "projectwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include "task.h"
#include "taskwindow.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>



// Цвета: Средний зеленый -  #a7bfa5, светлый зеленый - #e1f0db, темный зеленый - #3b4f2a


MainWindow::MainWindow(QWidget *parent) : QWidget(parent), scrollArea(new QScrollArea(this)) {
    setWindowTitle("Main Window");
    resize(1200, 700);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Белая шапка
    QLabel *topPart = new QLabel();
    topPart->setStyleSheet("background-color: white;");
    topPart->setFixedHeight(60);
    topPart->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(topPart);

    QHBoxLayout *headerLayout = new QHBoxLayout(topPart); // Горизонтальный слой

    // Стиль кнопок
    QString buttonStyle = "background-color: #3b4f2a; color: white; font-weight: bold; outline: none; border: none; border-radius: 5px; padding: 10px;";

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
    headerLayout->addStretch(1);

    // Название проекта Task Manager
    QLabel *title = new QLabel("Task Manager", this);
    title->setStyleSheet("background-color: transparent; color: black; font-weight: bold; font-size: 30px;");
    headerLayout->addWidget(title);

    // Добавляем пустое пространство справа для выравнивания
    headerLayout->addStretch(1);

    // Кнопка входа
    QPushButton *authLoginButton = new QPushButton("Войти", this);
    authLoginButton->setFixedSize(100, 35);
    authLoginButton->setStyleSheet(buttonStyle);
    // addShadowEffect(authLoginButton); // Добавляем тень

    headerLayout->addWidget(authLoginButton, 0, Qt::AlignRight);
    connect(authLoginButton, &QPushButton::clicked, this, &MainWindow::on_authLoginButton_clicked);

    // Кнопка регистрации
    QPushButton *regButton = new QPushButton("Зарегистрироваться", this);
    regButton->setFixedSize(150, 35);
    regButton->setStyleSheet(buttonStyle);
    // addShadowEffect(regButton); // Добавляем тень

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
    Task *newTask = new Task(this);

    tasksLayout->addWidget(newTask);

    // Центрируем все задачи по верхнему краю контейнера
    tasksLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

// Слот для создания нового проекта
void MainWindow::createNewProject() {
    Project *newProject = new Project(this);

    tasksLayout->addWidget(newProject);

    // Центрируем все проекты по верхнему краю контейнера
    tasksLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

// Нажатие на кнопку для перехода к окну регистрации
void MainWindow::on_authLoginButton_clicked()
{
    LoginWindow *loginWindow = new LoginWindow(this);
    loginWindow->show(); // Отображается поверх окна логина, можно потом пофиксить
}

// Нажатие на кнопку для перехода к окну регистрации
void MainWindow::on_regButton_clicked()
{
    RegistrationWindow *registerWindow = new RegistrationWindow(this);
    registerWindow->show(); // Отображается поверх окна логина, можно потом пофиксить
}

