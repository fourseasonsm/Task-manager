#include <QApplication>

#include "server.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    ChatServer server;
    server.startServer();

    return app.exec();
}
