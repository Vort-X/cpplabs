QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientobject.cpp \
    emailserver.cpp \
    main.cpp \
    requestprocessor.cpp \
    startwindow.cpp

HEADERS += \
    clientobject.h \
    emailserver.h \
    requestprocessor.h \
    startwindow.h

FORMS += \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-WGMailData-Desktop-Debug/release/ -lWGMailData
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-WGMailData-Desktop-Debug/debug/ -lWGMailData

INCLUDEPATH += $$PWD/../WGMailData
DEPENDPATH += $$PWD/../WGMailData
