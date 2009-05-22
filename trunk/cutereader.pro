TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += ./lib

QT += network xml webkit

UI_DIR=./tmpdata
MOC_DIR=./tmpdata
OBJECTS_DIR=./tmpdata
RCC_DIR=./tmpdata

# src
HEADERS += src/mainwin.h
FORMS += src/rssurl.ui src/whole.ui
SOURCES += src/main.cpp src/mainwin.cpp
RESOURCES += src/reader.qrc

# lib
HEADERS += lib/httpget.h lib/rsschannel.h
SOURCES += lib/httpget.cpp lib/rsschannel.cpp
