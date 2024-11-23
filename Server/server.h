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
<<<<<<< HEAD
private:
    QList<QTcpSocket*> clients;   // Список подключенных клиентов
    QHash<QString, QString> validUsers ;  // Хранилище логинов и паролей (логин -> пароль)
=======
    QList<QTcpSocket*> clients; // Список подключенных клиентов
>>>>>>> de6eec12417335b64cafc028f5d82ba561aeb9fa

private slots:
    void readMessage();
    void clientDisconnected();
    void registerUser(QTcpSocket *clientSocket, const QString &login, const QString &email, const QString &password); // Новый слот для регистрации
    void authenticateUser(QTcpSocket *clientSocket, const QString &login, const QString &password); // Новый слот для авторизации
};

#endif // SERVER_H
