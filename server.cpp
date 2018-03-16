#include "server.h"
#include "textoutput.h"
#include "mainwindow.h"

Server::Server(MainWindow *mainWindow, QWidget *parent) : QWidget(parent)
{
    m_wnd = mainWindow;
    vbl     = new QVBoxLayout;
    hbl     = new QHBoxLayout;
    te_out  = new TextOutput;

    btn  = new QPushButton("Stop");
    btn_clear = new QPushButton("Clear");

    hbl->setAlignment(Qt::AlignRight);
    hbl->addWidget(btn_clear);
    hbl->addWidget(btn);

    vbl->addLayout(hbl);
    vbl->addWidget(te_out);
    vbl->setMargin(0);
    setLayout(vbl);

    QObject::connect(btn, SIGNAL(clicked()), SLOT(toggleBtn()));
    QObject::connect(btn_clear, SIGNAL(clicked()), te_out, SLOT(clear()));
}

void Server::echo(QString msg)
{
    te_out->append(QTime::currentTime().toString() + " " + msg);
}

void Server::toggleBtn()
{
    if (btn->text() == "Stop") {
        btn->setText("Start");
        m_wnd->stopServer();
    } else {
        btn->setText("Stop");
        m_wnd->startServer();
    }
}
