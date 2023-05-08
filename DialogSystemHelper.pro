QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backgroundeditdialog.cpp \
    charactersview.cpp \
    choiceseditdialog.cpp \
    clickablelabel.cpp \
    conditioneditor.cpp \
    datatypes.cpp \
    dictionaryview.cpp \
    eventlistitem.cpp \
    main.cpp \
    mainwindow.cpp \
    personeditor.cpp \
    soundeditdialog.cpp

HEADERS += \
    backgroundeditdialog.h \
    charactersview.h \
    choiceseditdialog.h \
    clickablelabel.h \
    conditioneditor.h \
    datatypes.h \
    dictionaryview.h \
    eventlistitem.h \
    mainwindow.h \
    personeditor.h \
    soundeditdialog.h

FORMS += \
    backgroundeditdialog.ui \
    charactersview.ui \
    choiceseditdialog.ui \
    conditioneditor.ui \
    dictionaryview.ui \
    eventlistitem.ui \
    mainwindow.ui \
    personeditor.ui \
    soundeditdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
