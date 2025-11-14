QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    activationmanager.cpp \
    dialog/ActivationDialog.cpp \
    dialog/fluiddialog.cpp \
    dialog/materialdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    materialmanager.cpp \
    pipeflowmodule.cpp

HEADERS += \
    TYModule.h \
    activationmanager.h \
    common.h \
    dialog/ActivationDialog.h \
    dialog/fluiddialog.h \
    dialog/materialdialog.h \
    mainwindow.h \
    materialmanager.h \
    pipeflowmodule.h \
    thirdparty/include/CoolPropLib.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/lib/ -llibCoolProp.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/lib/ -llibCoolProp.dll

INCLUDEPATH += $$PWD/thirdparty/include
DEPENDPATH += $$PWD/thirdparty/include
