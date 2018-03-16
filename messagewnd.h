#ifndef MESSAGEWND_H
#define MESSAGEWND_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class MessageWnd : public QWidget
{
    Q_OBJECT
    QString m_message;
    QLabel *lbl;
    QHBoxLayout *hbl;
public:
    explicit MessageWnd(QString message, QWidget *parent = nullptr);
    QString message() const;
    void setMessage(const QString &message);
    virtual void show(QString);
signals:

public slots:

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // MESSAGEWND_H
