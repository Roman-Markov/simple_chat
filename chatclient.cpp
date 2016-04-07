#include <QtGui>
#include <QtWidgets>
#include "chatclient.h"

ChatClient::ChatClient(const QString strhost, int nPort, QWidget *pwgt):
    QWidget(pwgt), nBlockSize(0)
{
    pTcpSocket = new QTcpSocket(this);
    pTcpSocket->connectToHost(strhost, nPort);
    connect(pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));
    ptxt = new QTextEdit;
    pInput = new QLineEdit;
    ptxt->setReadOnly(true);

    QPushButton* pcmdSend = new QPushButton("&Send");
    connect(pcmdSend, SIGNAL(clicked()), SLOT(slotSendToServer()));
    connect(pInput, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
    pvbxLayout->addWidget(ptxt);
    pvbxLayout->addWidget((pInput));
    pvbxLayout->addWidget(pcmdSend);
    setLayout(pvbxLayout);
}

void ChatClient::slotReadyRead(){
    QDataStream in(pTcpSocket);
    in.setVersion(QDataStream::Qt_5_5);
    for(;;){
        if(!nBlockSize){
            if(pTcpSocket->bytesAvailable() < (int) sizeof(quint16))
                break;
            in >> nBlockSize;
        }
        if(pTcpSocket->bytesAvailable() < nBlockSize)
            break;
        QTime time;
        QString str;
        in >> time >> str;
        ptxt->append(time.toString() + " " + str);
        nBlockSize = 0;
    }
}

void ChatClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError?
                             "The host was not found.":
                             err == QAbstractSocket::RemoteHostClosedError?
                             "The remote host is closed.":
                             err == QAbstractSocket::ConnectionRefusedError?
                             "The connection was refused.":
                             QString(pTcpSocket->errorString()));
    ptxt->append(strError);
}

void ChatClient::slotSendToServer(){
QByteArray ar;
QDataStream out(&ar, QIODevice::WriteOnly);
out.setVersion(QDataStream::Qt_5_5);
out << quint16(0) << QTime::currentTime() << pInput->text();
out.device()->seek(0);
out << quint16(ar.size() - sizeof(quint16));

pTcpSocket->write(ar);
pInput->setText("");
}

void ChatClient::slotConnected(){
    ptxt->append("Received the connected() signal");
}




























