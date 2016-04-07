#ifndef LOGIN
#define LOGIN

#include <QtWidgets>
#include "chatclient.h"

class Login: public QWidget{
Q_OBJECT
private:
    QLabel* plabel;
    QLineEdit* pline;
    QPushButton* pcmd;
    ChatClient* pChatClient;
    QVBoxLayout* pvbx;
    QHBoxLayout* phbx;
public:
    Login(QWidget* pwgt = 0);
    void sendName();
public slots:
    void slotSendLogin();
};

#endif // LOGIN

