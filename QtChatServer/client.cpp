#include <QWidget>
#include <QString>
#include "client.h"

Client::Client(const Client& client){
    UserName = client.UserName;
    pSocket = client.pSocket;
}

Client::Client(QTcpSocket *ps){
    pSocket = ps;
    UserName = "whoiam";
}

QTcpSocket* Client::socket(){
    return pSocket;
}

QTcpSocket* Client::operator=(QTcpSocket* psock){
    pSocket = psock;
}

QString Client::operator=(QString* pstr){
    UserName = *pstr;
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
