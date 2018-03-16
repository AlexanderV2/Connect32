#pragma once
#define DEFAULT_MSG_DONE "OK."
#include "server.h"
#include "client.h"
#include <QtNetwork>
#include <messagewnd.h>
class QTcpSocket;
class QTcpServer;


class MainWindow : public QMainWindow
{
    Q_OBJECT
    Server *server;
    Client *client;
    QTcpSocket *socket; // server socket
    QTcpSocket *socket2; // client socket
    QTcpServer *tcpServer;
    QProcess *m_process;
    MessageWnd *message;
#ifndef Q_OS_ANDROID
    QSystemTrayIcon *tray;
    QMenu *trayMenu;
        QAction *ma_quit;
        bool blink;
        QTimer *timer;
#endif
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTabWidget *tabs;
    QSettings *settings;
    void sendToClient(QString str);
    void setIcon(int id);
public slots:
    void sendToServer();
    void connect();
    void disconnect();
    void newConnection();
    void disconnected();
    void readyRead();
    void readyRead2(); // for client
    void startServer();
    void stopServer();
    void error(QAbstractSocket::SocketError err);
    void commandDone(int code);
    void readStdout();
    void tabChanged(int param);
#ifndef Q_OS_ANDROID
    void blinkIcon(bool);
    void timeout();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
#endif
};
