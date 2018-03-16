#pragma once
#include <QTextEdit>

class TextOutput : public QTextEdit
{
public:
    TextOutput(QString text = "", QWidget *parent = 0);
    //virtual void append(QString text);
};

