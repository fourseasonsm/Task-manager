#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QTextStream>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

ChatServer::ChatServer(QObject *parent) : QTcpServer(parent) {
    // подключение к базе
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost"); //адрес моего сервера PostgreSQL
    db.setDatabaseName("task_manager"); // название базы
    db.setUserName("postgres"); // тут я указала свое имя пользователя для этой базы
    db.setPassword("password"); //пароль не указывала потому что хз как это запустить

    if (!db.open()) {
        qDebug() << "Не удалось подключиться к базе данных:" << db.lastError().text();
    }
}

void ChatServer::startServer() {
    QHostAddress serverAddress("192.168.1.147"); // Указываем IP-адрес
    quint16 port = 45561; // Указываем порт

    if (listen(serverAddress, port)) {
        qDebug() << "Сервер запущен на" << serverAddress.toString() << ", порт: " << port;
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

            //ищем логин пароль в базе
            QSqlQuery query;
            query.prepare("SELECT password FROM users WHERE login = :login");
            query.bindValue(":login", login);
            if (!query.exec()) {
                qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
                clientSocket->write("AUTH_FAILEDn");
                return;
            }

            if (query.next()) {
                QString storedPassword = query.value(0).toString();
                if (storedPassword == password) {
                    clientSocket->write("AUTH_SUCCESSn");
                } else {
                    clientSocket->write("AUTH_FAILEDn");
                }
            } else {
                clientSocket->write("AUTH_FAILEDn");
            }
        } else {
            clientSocket->write("INVALID_FORMATn");
        }
    }
}

void ChatServer::clientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    clients.removeAll(clientSocket);
    clientSocket->deleteLater();
    qDebug() << "Клиент отключен";
}
