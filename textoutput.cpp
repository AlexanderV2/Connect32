#include "textoutput.h"
#include <QTextCodec>
TextOutput::TextOutput(QString text, QWidget *parent) : QTextEdit(text, parent)
{
    this->setReadOnly(true);
}
/*
void TextOutput::append(QString text)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString utf8 = codec->toUnicode(text.toStdString().c_str());
    QTextEdit::insertPlainText(utf8);
}
*/
