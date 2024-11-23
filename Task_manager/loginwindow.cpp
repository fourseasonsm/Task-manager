#include "loginwindow.h"
#include "registrationwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , authenticated(false)
    , socket(new QTcpSocket(this))  // Инициализация сокета
{
    setWindowTitle("Авторизация");
    resize(300, 300);

    // Установка фона для всего окна
    setStyleSheet("background-color: #a7bfa5;");

    // Главный компоновщик
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Отступ сверху
    layout->addSpacing(10);

    // Заголовок
    QLabel *title = new QLabel("Авторизация");
    title->setStyleSheet("color: black; font-size: 30px;"); // font-family: 'Noto Sans' - заеб какой-то пока со шрифтами
    layout->addWidget(title, 0, Qt::AlignTop | Qt::AlignCenter);

    // Отступ
    layout->addSpacing(10);

    // Стиль полей
    QString lineEditStyle = "background-color: #e1f0db; color: black; font-size: 14px; outline: none; border: none;";

    // Поле логина
    loginLineEdit = new QLineEdit(this);
    loginLineEdit->setFixedSize(230, 30);
    loginLineEdit->setStyleSheet(lineEditStyle);
    loginLineEdit->setPlaceholderText("Введите логин"); // Текст-заполнитель
    layout->addWidget(loginLineEdit, 0, Qt::AlignCenter);

    // Расстояние между полями логина и пароля
    layout->addSpacing(20);

    // Поле пароля
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setFixedSize(230, 30);
    passwordLineEdit->setStyleSheet(lineEditStyle);
    passwordLineEdit->setPlaceholderText("Введите пароль"); // Текст-заполнитель
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordLineEdit, 0, Qt::AlignCenter);

    // Отступ преред кнопками
    layout->addSpacing(15);

    // Стиль кнопок
    QString buttonStyle = "background-color: #3b4f2a; color: white; font-weight: bold; outline: none; border: none; border-radius: 5px; padding: 10px;";

    // Кнопка входа
    QPushButton *authLoginButton = new QPushButton("Войти", this);
    authLoginButton->setFixedSize(150, 35);
    authLoginButton->setStyleSheet(buttonStyle);
    addShadowEffect(authLoginButton); // Добавляем тень

    layout->addWidget(authLoginButton, 0, Qt::AlignCenter | Qt::AlignBottom);
    connect(authLoginButton, &QPushButton::clicked, this, &LoginWindow::on_authLoginButton_clicked);

    // пока пока сервер
    // connectToServer();

    // Кнопка регистрации
    QPushButton *regButton = new QPushButton("Зарегистрироваться", this);
    regButton->setFixedSize(150, 35);
    regButton->setStyleSheet(buttonStyle);
    addShadowEffect(regButton); // Добавляем тень

    layout->addWidget(regButton, 0, Qt::AlignCenter | Qt::AlignBottom);
    connect(regButton, &QPushButton::clicked, this, &LoginWindow::on_regButton_clicked);

    // Расстояние между сообщением и кнопками
    layout->addSpacing(10);

    // Сообщение о соединении
    connectionStatusLabel = new QLabel("Соединение не установлено", this);
    connectionStatusLabel->setStyleSheet("color: black");
    layout->addWidget(connectionStatusLabel, 0, Qt::AlignCenter | Qt::AlignBottom);

    // Установка главного компоновщика
    setLayout(layout);
}

// Функция для добавления тени кнопке
void LoginWindow::addShadowEffect(QWidget *widget) {
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(widget);
    shadowEffect->setBlurRadius(10);                // Радиус размытия
    shadowEffect->setOffset(3, 3);                  // Смещение тени
    shadowEffect->setColor(QColor(50, 50, 50));     // Цвет тени
    widget->setGraphicsEffect(shadowEffect);
}

LoginWindow::~LoginWindow() {
    socket->disconnectFromHost();
}

void LoginWindow::sendCredentialsToServer() {
    QString login = loginLineEdit->text();
    QString password = passwordLineEdit->text();
    QString credentials = login + ":" + password;

    // Удаляем проверку состояния сокета
    // if (socket->state() == QAbstractSocket::ConnectedState) {
    // socket->write(credentials.toUtf8() + "n");
    // }
}

// Нажатие на кнопку для перехода к окну регистрации
void LoginWindow::on_regButton_clicked()
{
    RegistrationWindow *registerWindow = new RegistrationWindow(this);
    registerWindow->show(); // Отображается поверх окна логина, можно потом пофиксить
}

void LoginWindow::on_authLoginButton_clicked() {
    // заходим без проверки
    authenticated = true;
    accept();
    /*
    sendCredentialsToServer();

    if (socket->waitForReadyRead(3000)) {
        QString response = QString::fromUtf8(socket->readAll()).trimmed();
        if (response == "AUTH_SUCCESS") {
            authenticated = true;
            accept();  // Закрываем окно и разрешаем доступ
        } else if (response == "AUTH_FAILED") {
            QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
        } else {
            QMessageBox::warning(this, "Ошибка", "Некорректный ответ от сервера");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера");
    }
    */
}

void LoginWindow::connectToServer() {
    // без соединения с сервером
}
