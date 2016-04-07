#pragma once

#include <QWidget>

class QTcpServer;
class QTextEdit;
class QTcpSocket;

class ChatServer: public QWidget{
Q_OBJECT
private:
    QTcpServer* ptcpServer;
    QTextEdit* ptxt;
    quint16 nBlockSize;
private:
    void sendToClient(QTcpSocket* pClientSocket, const QString& str);
public:
    ChatServer(int numport, QWidget* pwgt = 0);
public slots:
    virtual void slotNewConnection();
    void slotReadClient();
};

