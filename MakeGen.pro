QT       += core

QT       -= gui

TARGET = MakeGen
CONFIG   += console

TEMPLATE = app


SOURCES += main.cpp \
    settings.cpp

OTHER_FILES += \
    Todo.txt

HEADERS += \
    settings.h \
    os.h
