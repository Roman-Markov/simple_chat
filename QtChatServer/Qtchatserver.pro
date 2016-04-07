TEMPLATE     = app
SOURCES += main.cpp \
chatserver.cpp
QT      += widgets network
HEADERS = chatserver.h
windows:TARGET = ../Server
