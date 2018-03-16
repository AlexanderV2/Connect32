#include "mainwindow.h"
//TODO out of tabs

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
#ifndef Q_OS_ANDROID
    timer = new QTimer(this);
    blink = false;
    tray = new QSystemTrayIcon(this);
    trayMenu = new QMenu(this);
        ma_quit = new QAction("Quit"); QObject::connect(ma_quit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    trayMenu->addAction(ma_quit);

    QObject::connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    tray->setContextMenu(trayMenu);

#ifndef Q_OS_ANDROID
    setIcon(3);
#endif

    tray->show();

    QObject::connect(timer, SIGNAL(timeout()), SLOT(timeout()));
#endif

    message = new MessageWnd("");
    settings = new QSettings("AlexanderV2", "Connect32", this);
    m_process = new QProcess(this);
    tcpServer = new QTcpServer();
    socket = 0;
    socket2 = new QTcpSocket(this);

    server = new Server(this, this);
    client = new Client(this, this);
    tabs = new QTabWidget(this);


    tabs->addTab(client, "Client");
    tabs->addTab(server, "Server");
    tabs->setTabPosition(QTabWidget::West);



    QObject::connect(tcpServer, SIGNAL(newConnection()),                        SLOT(newConnection()));
    QObject::connect(m_process, SIGNAL(finished(int)),                          SLOT(commandDone(int)));
    QObject::connect(m_process, SIGNAL(readyReadStandardOutput()),              SLOT(readStdout()));
    QObject::connect(socket2,   SIGNAL(connected()),            client,         SLOT(showInterface2()));
    QObject::connect(socket2,   SIGNAL(disconnected()),         client,         SLOT(showInterface1()));
    QObject::connect(socket2,   SIGNAL(error(QAbstractSocket::SocketError)),    SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(socket2,   SIGNAL(readyRead()),                            SLOT(readyRead2()));
    QObject::connect(tabs,      SIGNAL(currentChanged(int)),                    SLOT(tabChanged(int)));

    setCentralWidget(tabs);
#ifndef Q_OS_ANDROID
    resize(400,500);
    setWindowState(Qt::WindowMinimized);
#endif

     startServer();
}

MainWindow::~MainWindow(){}

void MainWindow::sendToClient(QString str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << quint16(0) << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}

void MainWindow::setIcon(int id)
{
#ifndef Q_OS_ANDROID
   tray->setIcon(QIcon(":/icons/"+QString::number(id)+".png"));
#endif
}

void MainWindow::sendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    out << quint16(0) << client->input();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    socket2->write(arrBlock);
    client->msg(client->input());
}


void MainWindow::connect() // connect to Server
{
    if (socket2->isOpen()) {
        socket2->abort();
        socket2->close();
        client->msg("Aborted.");
    }

    client->msg("Connecting...");
    settings->setValue("IP", client->input());
    socket2->connectToHost(client->input(), 2819);
}

void MainWindow::disconnect()
{
    socket->close();
    socket2->close();
}

void MainWindow::newConnection()
{
    QTcpSocket *tmp = tcpServer->nextPendingConnection();
    if (socket && socket->isOpen()) {
        server->echo("Someone is already connected");
        tmp->close();
        setIcon(3);
        return;
    }
    socket = tmp;
    server->echo("Client connected (" + socket->peerAddress().toString() + ")");
    sendToClient("Connected to server (" + socket->localAddress().toString() + ")");
    QObject::connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
    QObject::connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));

    setIcon(2);
}

void MainWindow::disconnected()
{
    socket->close();
    server->echo("Client disconnected (" + socket->peerAddress().toString() + ")");
    setIcon(3);
}


void MainWindow::startServer()
{
    if (tcpServer->listen(QHostAddress::AnyIPv4, 2819)){
        server->echo("Server has started");
        setIcon(2);
    } else {
        server->echo("Error: unable to start server.");
        setIcon(3);
    }
}

void MainWindow::stopServer()
{

    if (socket != 0 && socket->isOpen() ) {
        socket->close();
    }
    tcpServer->close();
    server->echo("Server closed.");
    setIcon(3);
}


void MainWindow::error(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                             "The host was not found." :
                             err == QAbstractSocket::RemoteHostClosedError ?
                                 "The remote host is closed." :
                                 err == QAbstractSocket::ConnectionRefusedError ?
                                     "The connection was refused." :
                                     QString(socket2->errorString())
                                     );
    socket2->close();
    client->msg(strError);
}

void MainWindow::commandDone(int code)
{
    sendToClient("Finished. Code=" + QString::number(code));
    m_process->close();
#ifndef Q_OS_ANDROID
    blinkIcon(0);
#endif
}


void MainWindow::readStdout()
{
    sendToClient( m_process->readAllStandardOutput() );
}

void MainWindow::tabChanged(int param)
{
#ifndef Q_OS_ANDROID
    if (param == 1){ // Server

        if (!tcpServer->isListening()){
            setIcon(3);
        } else {
             setIcon(2);
        }
    }
    else if(param == 0) { // Client tab
        if (socket2){
            setIcon( socket2->isOpen() ? 2 : 3 );
        }
    }
#endif
}

#ifndef Q_OS_ANDROID
void MainWindow::blinkIcon(bool b)
{

    if (b && !timer->isActive() ) {
        timer->start(300);
    } else {
        timer->stop();
        setIcon(2);
    }

}


void MainWindow::timeout()
{

    setIcon(blink);
    blink = !blink;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->activateWindow();
        this->setWindowState(Qt::WindowActive);
        this->show();
    break;
    default:;
    }

}
#endif
void MainWindow::readyRead() // server
{
    if (socket && socket->isOpen()) {
       QString addr = socket->peerAddress().toString();
       QDataStream in(socket);
       in.setVersion(QDataStream::Qt_5_9);
       quint16 nbs = 0; // next block size

       if ( socket->bytesAvailable() < sizeof(quint16) ) {
           server->echo("Error: code 0");
           return;
       }

       in >> nbs;
       if ( socket->bytesAvailable() < nbs ){
           server->echo("Error: code 1");
       }

       QString str;
       in >> str;
#ifndef Q_OS_ANDROID
       blinkIcon(1);
#endif
       server->echo("[" + addr + "]: " + str);

       if (str == "poweroff")
       {
           sendToClient("Powering off...");
           system("shutdown /f /s /t 3");
           exit(0);
       } else if (str == "msg") {
           message->close();
           commandDone(0);
       }
       else if (str.length() > 4) {
           QString op = QString::fromStdWString(str.toStdWString().substr(0, 3));
           QString ec = QString::fromStdWString(str.toStdWString().substr(4, str.size()-1));\
           if (op == "cmd") {
               if (m_process->isOpen()) {
                   m_process->close();
                   sendToClient("Process has been killed.");
               }

               m_process->start(op + " /C " + ec);

           } else if (op == "msg") {
                message->show(ec);
                commandDone(0);
           } else if (op == "psw") {
               if (m_process->isOpen()) {
                   m_process->close();
               }
               m_process->start("cmd /C net user %username% "+ec);
           } else if (op == "kil") {
               if (m_process->isOpen()) {
                   m_process->close();
               }
               m_process->start("cmd /C taskkill /f /im "+ec+".exe");
           }
        }
    }
}

void MainWindow::readyRead2() // read client
{
    quint16 nbs = 0;
    QDataStream in(socket2);
    in.setVersion(QDataStream::Qt_5_9);
    for (;;) {
        if (!nbs) {
            if (socket2->bytesAvailable() < (int)sizeof(quint16)) {
                break;
            }
            in >> nbs;
        }

        if (socket2->bytesAvailable() < nbs) {
            break;
        }
        QString str;
        in >> str;

        client->msg("[server] " + str);
        nbs = 0;
    }
}
