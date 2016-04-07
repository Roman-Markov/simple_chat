#include <QtWidgets>
#include "chatserver.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    ChatServer myserver(2323);
    myserver.show();
    return app.exec();
}
