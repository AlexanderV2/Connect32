#include "messagewnd.h"
#include <QFont>
QString MessageWnd::message() const
{
    return m_message;
}

void MessageWnd::setMessage(const QString &message)
{
    m_message = message;
    lbl->setText(message);
}

void MessageWnd::show(QString str)
{
    setMessage(str);
    QWidget::showFullScreen();
}

void MessageWnd::mousePressEvent(QMouseEvent *)
{
    this->close();
}

MessageWnd::MessageWnd(QString message, QWidget *parent) : QWidget(parent)
{
    hbl = new QHBoxLayout;
    lbl = new QLabel(message);
    lbl->setFont(QFont("Arial", 32));
    lbl->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    setMessage(message);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    //setAttribute(Qt::WA_TranslucentBackground);
    hbl->addWidget(lbl);
    setLayout(hbl);
    setWindowState(Qt::WindowActive);
    resize(500,250);
}
