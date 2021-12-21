QT    +=    core
QT    +=    gui
QT    += printsupport
QT    += networkauth
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    sources/dialogmail.cpp \
    sources/displayarea.cpp \
    sources/drive.cpp \
    sources/image.cpp \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/pdfhandler.cpp \
    #tests/test_image.cpp \
    #tests/test_mail.cpp

HEADERS += \
    headers/displayarea.h \
    headers/drive.h \
    headers/image.h \
    headers/mainwindow.h \
    headers/pdfhandler.h \
    headers/dialogmail.h \
    #tests/catch.hpp

FORMS += \
    forms/mainwindow.ui \
    forms/dialogmail.ui


INCLUDEPATH += ../PDFNetC64/Headers
INCLUDEPATH += -I/usr/include/x86_64-linux-gnu/curl

LIBS += -lcurl
LIBS+= -L../PDFNetC64/Lib -lPDFNetC -lstdc++ -lpthread -lm -lc -Wl,-rpath,../PDFNetC64/Lib -Wl,-rpath

#LIBS += -L/usr/lib/x86_64-linux-gnu/ -lcurl

#LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/ -lcurl


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc
