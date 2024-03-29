#-------------------------------------------------
#
# Project created by QtCreator 2018-07-12T13:53:59
#
#-------------------------------------------------

QT       += core gui
QT       += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IslandGame
TEMPLATE = app
CONFIG += c++14


SOURCES += \
    mainwindow.cpp \
    gameboard.cpp \
    player.cpp \
    hexgraphics.cpp \
    gamestate.cpp \
    startdialog.cpp \
    main.cc \
    pawngraphics.cpp \
    wheel.cpp \
    pixmapgraphics.cpp \
    resultdialog.cpp \
    infobox.cpp



HEADERS  += \
    gameboard.hh \
    player.hh \
    gamestate.hh \
    mainwindow.hh \
    hexgraphics.hh \
    startdialog.hh \
    pawngraphics.hh \
    wheel.hh \
    pixmapgraphics.hh \
    resultdialog.hh \
    infobox.hh


INCLUDEPATH += $$PWD/../GameLogic/Engine
DEPENDPATH += $$PWD/../GameLogic/Engine

CONFIG(release, debug|release) {
   DESTDIR = release
}

CONFIG(debug, debug|release) {
   DESTDIR = debug
}

LIBS += -L$$OUT_PWD/../GameLogic/Engine
LIBS += -L$$OUT_PWD/../GameLogic/Engine/$${DESTDIR}/ -lEngine

 win32 {
    copyfiles.commands += @echo NOW COPYING ADDITIONAL FILE(S) for Windows &
    copyfiles.commands += @echo from \"$$_PRO_FILE_PWD_/../GameLogic/Assets\" to  \"$$OUT_PWD\" &
    copyfiles.commands += @call xcopy \"$$_PRO_FILE_PWD_/../GameLogic/Assets\" \"$$OUT_PWD//Assets\" /i /s /e /y
}
unix:!macx {
    copyfiles.commands += echo \"NOW COPYING ADDITIONAL FILE(S) for Linux\" &&
    copyfiles.commands += echo from $$_PRO_FILE_PWD_/../GameLogic/Assets to  $$OUT_PWD &
    copyfiles.commands += cp -r $$_PRO_FILE_PWD_/../GameLogic/Assets $$DESTDIR
}
 macx {
    copyfiles.commands += @echo \"NOW COPYING ADDITIONAL FILE(S) for MacOS\" &&
    copyfiles.commands += echo from $$_PRO_FILE_PWD_/../GameLogic/Assets to  $$OUT_PWD &
    copyfiles.commands += mkdir -p $$DESTDIR/IslandGame.app/Contents/MacOS &&
    copyfiles.commands += cp -r $$_PRO_FILE_PWD_/../GameLogic/Assets $$DESTDIR &&
    copyfiles.commands += cp -r $$_PRO_FILE_PWD_/../GameLogic/Assets $$DESTDIR/IslandGame.app/Contents/MacOS/
 }

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles

FORMS += \
    mainwindow.ui \
    startdialog.ui \
    resultdialog.ui

RESOURCES += \
    images.qrc
