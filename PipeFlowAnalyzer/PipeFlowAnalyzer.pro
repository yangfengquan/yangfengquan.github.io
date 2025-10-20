QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FluidAnalyzer.cpp \
    MaterialManager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ConditionParser.h \
    FluidAnalyzer.h \
    FormulaParser.h \
    MaterialManager.h \
    mainwindow.h

# 第三方库头文件路径
INCLUDEPATH += $$PWD/thirdparty/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/lib/ -lCoolProp
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/lib/ -lCoolProp
#else:unix: LIBS += -L$$PWD/thirdparty/ -lCoolProp



#INCLUDEPATH += $$PWD/thirdparty/lib
#DEPENDPATH += $$PWD/thirdparty/lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/lib/ -llibCoolProp.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/lib/ -llibCoolProp.dll

INCLUDEPATH += $$PWD/thirdparty/lib
DEPENDPATH += $$PWD/thirdparty/lib
