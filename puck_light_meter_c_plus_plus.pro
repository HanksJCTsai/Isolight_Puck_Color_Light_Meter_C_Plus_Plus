QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

ICON += logo/logo.ico

RESOURCES += \
              qdarkstyle/dark/darkstyle.qrc \
              src/_serial_port_.json


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/qdevicewatch/qdevicewatcher_mac.cpp \
    src/qwork/qwritecmd.cpp

HEADERS += \
    src/mainwindow.h \
    src/qdevicewatch/qdevicewatcher_mac.h \
    src/qwork/qwritecmd.h

FORMS += \
    src/mainwindow.ui

QT += \
    serialport

LIBS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
