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
    Client(const Client& cl);
    Client(QTcpSocket* ps);
    QTcpSocket* socket();
    QTcpSocket* operator=(QTcpSocket* psock);
    QString operator=(QString* pstr);
    Client operator=(Client* pclient);
    bool operator<(const Client& pclient)const;
    bool operator==(const Client& pclient)const;
    //bool friend operator < (const Client& c1, const Client& c2) const;
};

#endif // CLIENT

