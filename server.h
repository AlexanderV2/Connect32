// There is the Server GUI Description
#pragma once
#include <QtWidgets>
class MainWindow;
class TextOutput;
class Server : public QWidget
{
    Q_OBJECT
    MainWindow *m_wnd;
    QHBoxLayout *hbl;
    QVBoxLayout *vbl;
    QPushButton *btn, *btn_clear;
    TextOutput *te_out;
public:
    explicit Server(MainWindow *mainWindow, QWidget *parent = 0);
    void echo(QString msg);
signals:

public slots:
    void toggleBtn();
};
