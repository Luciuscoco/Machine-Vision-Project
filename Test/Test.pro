# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = Test
DESTDIR = ../Bin/x64_Debug
QT += core gui widgets
CONFIG += debug
DEFINES += WIN64 QT_DLL QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/$(ConfigurationName)
LIBS += -lZVisiond
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(Test.pri)