#Author: zhu,shiroki@www.cuteqt.com
#License: GPLv2.0

TEMPLATE = app
TARGET = cutereader
DEPENDPATH += .
INCLUDEPATH += ./lib
SUBDIRS+=./lib
#DEFINES+=RSS_DOCUMENT_SUPPORT
QT += network xml webkit

UI_DIR=./tmpdata
MOC_DIR=./tmpdata
OBJECTS_DIR=./tmpdata
RCC_DIR=./tmpdata

# src
HEADERS += src/mainwin.h src/channelmodel.h
FORMS += src/rssurl.ui src/whole.ui src/about.ui
SOURCES += src/main.cpp src/mainwin.cpp src/channelmodel.cpp
RESOURCES += src/reader.qrc

# lib
HEADERS += lib/rsschannel.h lib/rssdocument.h \
		lib/wget.h
SOURCES += lib/rsschannel.cpp lib/rssdocument.cpp\
		lib/wget.cpp

TRANSLATIONS+=i18n/cutereader-zh_CN.ts i18n/cutereader-zh_TW.ts
