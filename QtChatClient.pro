TEMPLATE     = app
QT      += widgets network
SOURCES += \
    main.cpp \
    chatclient.cpp

HEADERS += \
    chatclient.h
windows:TARGET = ../Client
