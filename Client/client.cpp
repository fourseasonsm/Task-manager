#include "client.h"
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Main Window");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Отображение сообщений
    chatDisplay = new QListWidget(this);
    mainLayout->addWidget(chatDisplay);

    // Поле для ввода сообщений
    messageInput = new QLineEdit(this);
    mainLayout->addWidget(messageInput);

    // Кнопка отправки
    sendButton = new QPushButton("Отправить", this);
    mainLayout->addWidget(sendButton);

    setLayout(mainLayout);

    // Создаем сокет для связи с сервером
    clientSocket = new QTcpSocket(this);

    // Подключаемся к серверу
    connectToServer();

    // Соединяем сигналы и слоты
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::receiveMessage);
}

MainWindow::~MainWindow() {
    clientSocket->close();
}

void MainWindow::connectToServer() {
    clientSocket->connectToHost("192.168.1.147", 45561); // Указываем нужный IP-адрес и порт
    if (clientSocket->waitForConnected(3000)) {
        chatDisplay->addItem("Подключено к серверу");
    } else {
        chatDisplay->addItem("Не удалось подключиться к серверу");
    }
}

void MainWindow::sendMessage() {
    QString message = messageInput->text();
    if (!message.isEmpty() && clientSocket->state() == QAbstractSocket::ConnectedState) {
        clientSocket->write(message.toUtf8() + "\n");
        messageInput->clear();
    }
}

void MainWindow::receiveMessage() {
    while (clientSocket->canReadLine()) {
        QString message = QString::fromUtf8(clientSocket->readLine()).trimmed();
        chatDisplay->addItem(message);
    }
}
