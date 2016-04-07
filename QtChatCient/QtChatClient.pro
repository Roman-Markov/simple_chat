TEMPLATE     = app
QT      += widgets network
SOURCES += \
    main.cpp \
    chatclient.cpp \
    login.cpp

HEADERS += \
    chatclient.h \
    login.h
windows:TARGET = ../Client
