// There is the Client GUI Description
#pragma once

#include <QtWidgets>
#include "textoutput.h"
class MainWindow;

class Client : public QWidget
{
    Q_OBJECT
    MainWindow  *m_wnd;
    QHBoxLayout *hbl, *hbl_top;
    QVBoxLayout *vbl;
    TextOutput  *te_out;
    QLineEdit   *le_input;
    QPushButton *btn,    // connect/send
                *btn2,   // disconnect
                *btn3;   // clear
    bool ac; // already was cannected
public:
    explicit Client(MainWindow *mainWindow, QWidget *parent = nullptr);
    void msg(QString str);
public slots:
    void showInterface1();
    void showInterface2();
    QString input() const;
};
