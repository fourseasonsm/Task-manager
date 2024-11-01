#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QObject *parent = nullptr);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QList<QTcpSocket*> clients; // Список подключенных клиентов

private slots:
    void readMessage();
    void clientDisconnected();
    void registerUser(QTcpSocket *clientSocket, const QString &login, const QString &password); // Новый слот для регистрации
    void authenticateUser(QTcpSocket *clientSocket, const QString &login, const QString &password); // Новый слот для авторизации
};

#endif // SERVER_H
