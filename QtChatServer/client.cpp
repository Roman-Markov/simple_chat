#include <QWidget>
#include <QString>
#include "client.h"

Client::Client(const Client& cl){
    UserName = cl.UserName;
    pSocket = cl.pSocket;
}

QString Client::name(){
    return UserName;
}

Client::Client(const QString& name, QTcpSocket* ps){
    UserName = name;
    pSocket = ps;
}

void Client::setSocket(QTcpSocket* ps){
    pSocket = ps;
}

QTcpSocket* Client::socket()const{
    return pSocket;
}

Client Client::operator=(QTcpSocket* psock){
    pSocket = psock;
    return *this;
}

Client Client::operator=(QString pstr){
    UserName = pstr;
    return *this;
}

Client Client::operator=(Client* pclient){
    pSocket = pclient->pSocket;
    UserName = pclient->UserName;
    return *this;
}

bool Client::operator<(const Client& pclient) const{
    return (this->UserName < pclient.UserName);
}

bool Client::operator==(const Client& client) const{
    return ((this->pSocket== client.pSocket) && (this->UserName == client.UserName));
}

//bool operator< (const Client& c1, const Client& c2){
//    return c1.UserName < c2.UserName;
//}
