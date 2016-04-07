#include <QtNetwork>
#include <QtWidgets>
#include "chatserver.h"
#include "client.h"
#include <QString>
#include <iostream>
#include <set>

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
    //Получение нового соединения и инициализация соответсвующего сокета
    QTcpSocket* pClientSocket = ptcpServer->nextPendingConnection();

    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient()));
    Client newClient;
    newClient.setSocket(pClientSocket);
    newclients.insert(newClient);
    ptxt->append("new connection");
    sendToClient("Server response: Connected!");
}

void ChatServer::slotReadClient(){
    QTcpSocket* pClientSocket = (QTcpSocket*) sender();
    foreach(Client client, clients){
        if(client.socket()->socketDescriptor() == pClientSocket->socketDescriptor()){
            readClient(pClientSocket);
            return;
        }
    }
    // не прошел проверку, т. е. новый, принимаем от него псевдоним
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
        QString str;
        in >> str;
        /*foreach(Client client, newclients){
            if(client.socket()->socketDescriptor() == pClientSocket->socketDescriptor()){
                client = str;
                clients.insert(client);
                newclients.erase(client);
                QString strmsg = QTime::currentTime().toString() + "New user - " + str;
                ptxt->append(strmsg);
                nBlockSize = 0;
                sendToClient(str + " joined us");
            }
        }*/
        int j = 0;
        std::set<Client>::iterator iter = newclients.begin();
        while( j < newclients.size()){
            if(iter->socket()->socketDescriptor() == pClientSocket->socketDescriptor()){
                Client temp(str, (*iter).socket());
                clients.insert(temp);
                newclients.erase(iter);
                QString strmsg = QTime::currentTime().toString() + "New user - " + str;
                ptxt->append(strmsg);
                nBlockSize = 0;
                sendToClient(str + " joined us");
            }
            j++;
            iter++;
        }
    }
}

void ChatServer::sendToClient(const QString& str){
    QByteArray ar;
    QDataStream out(&ar, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(ar.size() - sizeof(quint16));

    foreach(Client client, clients){
    client.socket()->write(ar);
    }
}

// чтение сообщений активных сокетов
void ChatServer::readClient(QTcpSocket* pClientSocket){
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
        sendToClient(str);
    }
}




