TEMPLATE     = app
SOURCES += main.cpp \
chatserver.cpp \
    client.cpp
QT      += widgets network
HEADERS = chatserver.h \
    client.h
windows:TARGET = ../Server
