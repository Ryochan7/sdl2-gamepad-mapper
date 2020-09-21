QT += quick quickcontrols2

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        applogger.cpp \
        apploggersingleton.cpp \
        displaymappedjoystickbackend.cpp \
        joystickcontainer.cpp \
        joystickcontainermodel.cpp \
        joysticksdl.cpp \
        main.cpp \
        mainviewbackend.cpp \
        mappinggcviewbackend.cpp \
        registerqmltypes.cpp \
        sdl2mapperbackend.cpp \
        sdlinputreader.cpp

RESOURCES += qml.qrc \
    resources.qrc

TRANSLATIONS += \
    sdl2-gamepad-mapper_en_US.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    applogger.h \
    apploggersingleton.h \
    displaymappedjoystickbackend.h \
    joystickcontainer.h \
    joystickcontainermodel.h \
    joysticksdl.h \
    mainviewbackend.h \
    mappinggcviewbackend.h \
    registerqmltypes.h \
    sdl2mapperbackend.h \
    sdlinputreader.h

win32: LIBS += -L$$PWD/SDL2/lib/ -llibSDL2.dll
else: LIBS += -lSDL2

windows: INCLUDEPATH += $$PWD/SDL2/include
windows: DEPENDPATH += $$PWD/SDL2/include

DISTFILES += \
    images/axis.png \
    images/button.png \
    images/controllermap.png \
    images/down-arrow-small.png \
    images/left-arrow-small.png \
    images/right-arrow-small.png \
    images/up-arrow-small.png
