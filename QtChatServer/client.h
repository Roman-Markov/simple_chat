#ifndef CLIENT
#define CLIENT

#include <QWidget>
#include <QString>
class QTcpSocket;

class Client{
private:
    QString UserName;
    QTcpSocket* pSocket;
public:
    Client(const QString& name = "whoiam", QTcpSocket* ps = 0);
    Client(const Client& cl);

    void setSocket(QTcpSocket* ps);
    QTcpSocket* socket()const;
    //QString& UserName();
    Client operator=(QTcpSocket* psock);
    Client operator=(QString pstr);
    Client operator=(Client* pclient);
    bool operator<(const Client& pclient)const;
    bool operator==(const Client& pclient)const;
    //bool friend operator < (const Client& c1, const Client& c2) const;
};

#endif // CLIENT

