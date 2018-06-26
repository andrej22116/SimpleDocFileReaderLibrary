TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    ClassBinaryStreamWrapper/binarystreamwrapper.cpp \
    ClassContainer/container.cpp \
    ClassContainer/ClassTextContainer/textcontainer.cpp \
    ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.cpp \
    ClassWindowsCompoundBinaryFileFormatReader/ClassStreamsMenager/streamsmenager.cpp \
    ClassInputBinaryStream/inputbinarystream.cpp

HEADERS += \
    ClassBinaryStreamWrapper/binarystreamwrapper.hpp \
    ClassContainer/container.h \
    ClassContainer/ClassTextContainer/textcontainer.h \
    ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.h \
    variablevisualize.hpp \
    WindowsCompoundBinaryFileformat/wcbff_structures.h \
    ClassWindowsCompoundBinaryFileFormatReader/ClassStreamsMenager/streamsmenager.h \
    stdincludes.h \
    ClassInputBinaryStream/inputbinarystream.h
