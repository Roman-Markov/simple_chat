#include <QtNetwork>
#include <QtWidgets>
#include "chatserver.h"

ChatServer::ChatServer(int numport, QWidget *pwgt): QWidget(pwgt), nBlockSize(0)
{
    ptcpServer = new QTcpServer(this);
    if(!ptcpServer->listen(QHostAddress::Any, numport)) {
        QMessageBox::critical(0,
                              "Server erro",
                              "Unable to begin server work: " + ptcpServer->errorString());
        ptcpServer->close();
        return;
    }

    connect(ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection()));
    ptxt = new QTextEdit;
    ptxt->setReadOnly(true);

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Server<H1>"));
    pvbxLayout->addWidget(ptxt);
    setLayout(pvbxLayout);
}

/*virtual*/ void ChatServer::slotNewConnection(){
    QTcpSocket* pClientSocket = ptcpServer->nextPendingConnection();

    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient()));

    sendToClient(pClientSocket, "Server response: Connected!");
}

void ChatServer::slotReadClient(){
    QTcpSocket* pClientSocket = (QTcpSocket*) sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_5);
    for(;;){
        if(!nBlockSize){
            if(pClientSocket->bytesAvailable() < (int) sizeof(quint16))
                break;
            in >> nBlockSize;
        }
        if(pClientSocket->bytesAvailable() < nBlockSize)
            break;
        QTime time;
        QString str;
        in >> time >> str;
        QString strmsg = time.toString() + " " + "Client has sent - " + str;
        ptxt->append(strmsg);
        nBlockSize = 0;
        sendToClient(pClientSocket, "Server response: Received\"" + str + "\"");
    }
}

void ChatServer::sendToClient(QTcpSocket* pClientSocket, const QString& str){
    QByteArray ar;
    QDataStream out(&ar, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(ar.size() - sizeof(quint16));

    pClientSocket->write(ar);
}







