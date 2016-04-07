#include <QtWidgets>
#include "chatclient.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    ChatClient myclient("localhost", 2323);
    myclient.show();
    return app.exec();
}
