#include "client.h"
#include "mainwindow.h"
Client::Client(MainWindow *mainWindow, QWidget *parent) : QWidget(parent)
{
    m_wnd = mainWindow;
    ac = false;
    te_out      = new TextOutput("", this);
    le_input    = new QLineEdit;
    btn         = new QPushButton;
    btn2        = new QPushButton("Disconnect");
    btn3        = new QPushButton("Clear");
    vbl         = new QVBoxLayout;
    hbl         = new QHBoxLayout;
    hbl_top     = new QHBoxLayout;

    hbl_top->setAlignment(Qt::AlignRight);
    hbl_top->addWidget(btn2);
    hbl_top->addWidget(btn3);


    hbl->addWidget(le_input);
    hbl->addWidget(btn);
    vbl->addLayout(hbl_top);
    vbl->addWidget(te_out);
    vbl->addLayout(hbl);
    setLayout(vbl);
    showInterface1();


    QObject::connect(btn2, SIGNAL(clicked()), m_wnd, SLOT(disconnect()));
    QObject::connect(btn3, SIGNAL(clicked()), te_out, SLOT(clear()));
}

void Client::msg(QString str)
{
     te_out->append(QTime::currentTime().toString() + " " + str);
}

void Client::showInterface1()
{
    le_input->setPlaceholderText("IP Address");
    le_input->setInputMask("000.000.000.000;-");
    le_input->setText(m_wnd->settings->value("IP", QVariant("192.168")).toString());
    btn->setText("Connect");
    btn2->setEnabled(false);

    QObject::connect(btn, SIGNAL(clicked()), m_wnd, SLOT(connect()));
    QObject::connect(le_input, SIGNAL(returnPressed()), m_wnd, SLOT(connect()));

    QObject::disconnect(btn, SIGNAL(clicked()), m_wnd, SLOT(sendToServer()));
    QObject::disconnect(le_input, SIGNAL(returnPressed()), m_wnd, SLOT(sendToServer()));
}

void Client::showInterface2()
{
    QObject::disconnect(btn, SIGNAL(clicked()), m_wnd, SLOT(connect()));
    QObject::disconnect(le_input, SIGNAL(returnPressed()), m_wnd, SLOT(connect()));
    QObject::connect(btn, SIGNAL(clicked()), m_wnd, SLOT(sendToServer()));
    QObject::connect(le_input, SIGNAL(returnPressed()), m_wnd, SLOT(sendToServer()));
    le_input->setPlaceholderText("Send to the server...");
    le_input->setInputMask("");
    btn->setText("Send");
    btn2->setEnabled(true);
}

QString Client::input() const
{
    return le_input->text();
}
