QT    +=    core
QT    +=    gui
QT += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/displayarea.cpp \
    src/image.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    headers/displayarea.h \
    headers/image.h \
    headers/mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += ../PDFNetC64/Headers
#LIBS += -L../PDFNetC64/Lib -lPDFNetC -lstdc++17 -lpthread -lm -lc -Wl,-rpath,../PDFNetC64/Lib -Wl,-rpath
LIBS+= -L../PDFNetC64/Lib -lPDFNetC -lstdc++ #-lpthread -lm -lc -Wl,-rpath,../PDFNetC64/Lib -Wl,-rpath

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc


