QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TreeItem.cpp \
    TreeItemDelegate.cpp \
    TreeModel.cpp \
    TreeWidget.cpp \
    JsonTplLoader.cpp \
    main.cpp \
    JsonEditor.cpp

HEADERS += \
    JsonEditor.h \
    TreeItem.h \
    TreeItemDelegate.h \
    TreeModel.h \
    TreeWidget.h \
    JsonTplLoader.h

FORMS += \
    JsonEditor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc
