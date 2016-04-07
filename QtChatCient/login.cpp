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
    pChatClient->show();
    this->close();
}
