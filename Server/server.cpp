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
    // Подключение к базе данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost"); //адрес моего сервера PostgreSQL
    db.setDatabaseName("task_manager"); // название базы
    db.setUserName("postgres"); // тут я указала свое имя пользователя для этой базы
    db.setPassword("miumiau"); //пароль не указывала потому что хз как это запустить

    if (!db.open()) {
        qDebug() << "Не удалось подключиться к базе данных:" << db.lastError().text();
    }
}

void ChatServer::startServer() {
    QHostAddress serverAddress("localhost"); // Указываем IP-адрес
    quint16 port = 8000; // Указываем порт

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

        QStringList parts = message.split(':');
        if (parts.size() < 3) {
            clientSocket->write("INVALID_FORMAT\n");
            return;
        }

        QString command = parts[0];
        QString login = parts[1];

        if (command == "REGISTER") {
            QString email = parts[2];
            QString password = parts[3];
            registerUser(clientSocket, login, email, password);
        } else if (command == "LOGIN") {
            QString password = parts[2];
            authenticateUser(clientSocket, login, password);
        } else {
            clientSocket->write("UNKNOWN_COMMAND\n");
        }
    }
}

void ChatServer::registerUser(QTcpSocket *clientSocket, const QString &login, const QString &email, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (login, email, password) VALUES (:login, :email, :password)");
    query.bindValue(":login", login);
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec()) {
        clientSocket->write("REGISTER_SUCCESS\n");
    } else {
        clientSocket->write("REGISTER_FAILED\n");
        qDebug() << "Ошибка регистрации пользователя:" << query.lastError().text();
    }
}

void ChatServer::authenticateUser(QTcpSocket *clientSocket, const QString &login, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE login = :login");
    query.bindValue(":login", login);

    qDebug () << login << ' ' << password;

    if (!query.exec()) {
        clientSocket->write("AUTH_FAILED\n");
        return;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            clientSocket->write("AUTH_SUCCESS\n");
        } else {
            clientSocket->write("AUTH_FAILED\n");
        }
    } else {
        clientSocket->write("AUTH_FAILED\n");
    }
}

void ChatServer::clientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    clients.removeAll(clientSocket);
    clientSocket->deleteLater();
    qDebug() << "Клиент отключен";
}
