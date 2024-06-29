QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addmenudialog.cpp \
    addtabledialog.cpp \
    adduserdialog.cpp \
    database.cpp \
    editmenudialog.cpp \
    edittabledialog.cpp \
    edituserdialog.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    menumanager.cpp \
    orderlists.cpp \
    reportmanager.cpp \
    tablemanager.cpp \
    usermanager.cpp

HEADERS += \
    addmenudialog.h \
    addtabledialog.h \
    adduserdialog.h \
    database.h \
    editmenudialog.h \
    edittabledialog.h \
    edituserdialog.h \
    login.h \
    mainwindow.h \
    menumanager.h \
    orderlists.h \
    reportmanager.h \
    tablemanager.h \
    usermanager.h

FORMS += \
    addmenudialog.ui \
    addtabledialog.ui \
    adduserdialog.ui \
    editmenudialog.ui \
    edittabledialog.ui \
    edituserdialog.ui \
    login.ui \
    mainwindow.ui \
    menumanager.ui \
    orderlists.ui \
    reportmanager.ui \
    tablemanager.ui \
    usermanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
