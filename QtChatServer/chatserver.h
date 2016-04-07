#pragma once

#include <QWidget>
#include <QSet>
class QTcpServer;
class QTextEdit;
class QTcpSocket;

class ChatServer: public QWidget{
Q_OBJECT
private:
    QSet<QTcpSocket*> clients;
    QTcpServer* ptcpServer;
    QTextEdit* ptxt;
    quint16 nBlockSize;
private:
    void sendToClient(const QString& str);
public:
    ChatServer(int numport, QWidget* pwgt = 0);
public slots:
    virtual void slotNewConnection();
    void slotReadClient();
};

