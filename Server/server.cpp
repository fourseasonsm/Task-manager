#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QTextStream>
#include <QList>

ChatServer::ChatServer(QObject *parent) : QTcpServer(parent) {
    // Логины и пароли
    validUsers.insert("111", "111");
    validUsers.insert("admin", "admin123");
}
void ChatServer::startServer() {
    QHostAddress serverAddress("192.168.1.147"); // Указываем IP-адрес
    quint16 port = 45561; // Указываем порт

    if (listen(serverAddress, port)) {
        qDebug() << "Сервер запущен на" << serverAddress.toString() << "порт" << port;
    } else {
        qDebug() << "Не удалось запустить сервер!";
    }
}

void ChatServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);
    clients.append(clientSocket);

    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::readMessage);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::clientDisconnected);

    qDebug() << "Новый клиент подключен!";
}

void ChatServer::readMessage() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    while (clientSocket->canReadLine()) {
        QString message = QString::fromUtf8(clientSocket->readLine()).trimmed();
        qDebug() << "Получено сообщение:" << message;

        // Разделяем сообщение на логин и пароль
        QStringList credentials = message.split(':');
        if (credentials.size() == 2) {
            QString login = credentials[0];
            QString password = credentials[1];

            // Проверка на валидность логина и пароля
            if (validUsers.contains(login) && validUsers.value(login) == password) {
                clientSocket->write("AUTH_SUCCESS\n");
            } else {
                clientSocket->write("AUTH_FAILED\n");
            }
        } else {
            clientSocket->write("INVALID_FORMAT\n");
        }
    }
}

void ChatServer::clientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    clients.removeAll(clientSocket);
    clientSocket->deleteLater();
    qDebug() << "Клиент отключен";
}
