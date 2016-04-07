#include <QtWidgets>
#include "chatclient.h"
#include "login.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Login user;
    user.show();
    //ChatClient myclient("localhost", 2323);
    //myclient.show();
    return app.exec();
}
