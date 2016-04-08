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
    newclients.insert(Client("whoiam" + QString::number(rand()%1000000000), pClientSocket));
    qint32 n = newclients.size();
    ptxt->append(QString::number(n) + " new connections");
    sendToClient("Server response: Connected!");
}

void ChatServer::slotReadClient(){
    QTcpSocket* pClientSocket = (QTcpSocket*) sender();
    for(std::set<Client>::iterator iter = clients.begin(); iter != clients.end(); iter++){
        if(iter->socket()->socketDescriptor() == pClientSocket->socketDescriptor()){
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
        int j = 0;
        // удаляем из новых, добавляем к активным
        std::set<Client>::iterator iter = newclients.begin();
        while( j < newclients.size()){
            if(iter->socket()->socketDescriptor() == pClientSocket->socketDescriptor()){
                Client temp(str, pClientSocket);
                clients.insert(temp);
                connect(temp.socket(), SIGNAL(disconnected ()),
                      this, SLOT(slotDisconnected()));
                newclients.erase(iter);

                qint32 n = newclients.size();
                ptxt->append("Осталось в логине: " + QString::number(n));

                QString strmsg = QTime::currentTime().toString() + " New user - " + str;
                ptxt->append(strmsg);
                nBlockSize = 0;
                sendToClient(str + " joined us");
                break;
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

    for(std::set<Client>::iterator iter = clients.begin(); iter != clients.end(); iter++){
        iter->socket()->write(ar);
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
        QString strname;
        foreach(Client client, clients){
            if(client.socket()->socketDescriptor() == pClientSocket->socketDescriptor()){
                strname = client.name();
                break;
            }
        }
        QString strhtml = "<H4><FONT COLOR=RED>" + strname
                + ":: </FONT><FONT COLOR=BLUE>" + str + "</FONT> <H4>";
        sendToClient(strhtml);
    }
}

void ChatServer::slotDisconnected()
{
    QTcpSocket* pClientSocket = (QTcpSocket*) sender();
    // Удаление испорченных сокетов
    foreach(Client client, clients){
        if(client.socket()->socketDescriptor() == pClientSocket->socketDescriptor()){
            ptxt->append(client.name() + " go out");
            clients.erase(client);
            break;
        }
    }
    qint32 n = clients.size();
    ptxt->append("Осталось активных: " + QString::number(n));
}


