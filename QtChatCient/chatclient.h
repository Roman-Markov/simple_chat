#ifndef CHATCLIENT_H
#define CHATCLIENT_H
#include <QWidget>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;

class ChatClient: public QWidget{
Q_OBJECT
private:
    QTcpSocket* pTcpSocket;
    QTextEdit* ptxt;
    QLineEdit* pInput;
    quint16 nBlockSize;
public:
    QString UserName;
    ChatClient(const QString strhost, int nPort, QString name = "", QWidget* pwgt = 0);
    QString& name();
    QTcpSocket* tcpsocket();
public slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
};

#endif // CHATCLIENT_H

