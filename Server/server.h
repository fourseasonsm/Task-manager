#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QList>

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QObject *parent = nullptr);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
private:
    QList<QTcpSocket*> clients;   // Список подключенных клиентов
    QHash<QString, QString> validUsers ;  // Хранилище логинов и паролей (логин -> пароль)

private slots:
    void readMessage();
    void clientDisconnected();
};

#endif // SERVER_H
