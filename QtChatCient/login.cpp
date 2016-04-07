#include <QtWidgets>
#include <login.h>
#include "chatclient.h"

Login::Login(QWidget *pwgt): QWidget(pwgt){
    plabel = new QLabel("Name:");
    pline = new QLineEdit;
    pcmd = new QPushButton("OK");

    pChatClient = new ChatClient("localhost", 2323);
    pvbx = new QVBoxLayout;
    phbx = new QHBoxLayout;
    phbx->addWidget(plabel);
    phbx->addWidget(pline);
    pvbx->addLayout(phbx);
    pvbx->addWidget(pcmd);
    setLayout(pvbx);
    connect(pcmd, SIGNAL(clicked()), this, SLOT(slotSendLogin()));
    connect(pline, SIGNAL(returnPressed()), this, SLOT(slotSendLogin()));
}

void Login::slotSendLogin(){
    pChatClient->name() = pline->text();
    pChatClient->show();
    this->close();
    sendName();
}


void Login::sendName(){
    QByteArray ar;
    QDataStream out(&ar, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    QString username = pline->text();
    out << quint16(0) << username;
    out.device()->seek(0);
    out << quint16(username.size()+1 - sizeof(quint16));
    pChatClient->tcpsocket()->write(ar);
}
