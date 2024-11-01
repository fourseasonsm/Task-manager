#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void connectToServer();
    void sendMessage();
    void receiveMessage();

private:
    QTcpSocket *clientSocket;
    QLineEdit *messageInput;
    QListWidget *chatDisplay;
    QPushButton *sendButton;
};

#endif // CLIENT_H
