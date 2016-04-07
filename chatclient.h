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
    ChatClient(const QString strhost, int nPort, QWidget* pwgt = 0);
public slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
};

#endif // CHATCLIENT_H

