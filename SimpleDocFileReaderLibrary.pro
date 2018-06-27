TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

PRECOMPILED_HEADER = stdincludes.h

HEADERS += \
    ClassContainer/container.h \
    ClassInputBinaryStream/inputbinarystream.h \
    ClassTextContainer/textcontainer.h \
    ClassWindowsCompoundBinaryFileFormatReader/wcbff_structures.h \
    ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.h \
    ClassWordBinaryFileFormatReader/wbff_structures.h \
    ClassWordBinaryFileFormatReader/wordbinaryfileformatreader.h \
    variablevisualize.hpp

SOURCES += \
    ClassContainer/container.cpp \
    ClassInputBinaryStream/inputbinarystream.cpp \
    ClassTextContainer/textcontainer.cpp \
    ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.cpp \
    ClassWordBinaryFileFormatReader/wordbinaryfileformatreader.cpp \
    main.cpp
