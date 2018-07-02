TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

PRECOMPILED_HEADER = stdincludes.h

HEADERS += \
    ClassInputBinaryStream/inputbinarystream.h \
    ClassWindowsCompoundBinaryFileFormatReader/wcbff_structures.h \
    ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.h \
    ClassWordBinaryFileFormatReader/wbff_structures.h \
    ClassWordBinaryFileFormatReader/wordbinaryfileformatreader.h \
    variablevisualize.hpp \
    Containers/ClassTextContainer/textcontainer.h \
    Containers/ClassContainer/container.h \
    Containers/ClassImageContainer/imagecontainer.h \
    Containers/ClassTableContainer/tablecontainer.h \
    Containers/ClassTableCellContainer/tablecellcontainer.h

SOURCES += \
    ClassInputBinaryStream/inputbinarystream.cpp \
    ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.cpp \
    ClassWordBinaryFileFormatReader/wordbinaryfileformatreader.cpp \
    main.cpp \
    Containers/ClassTextContainer/textcontainer.cpp \
    Containers/ClassContainer/container.cpp \
    Containers/ClassImageContainer/imagecontainer.cpp \
    Containers/ClassTableContainer/tablecontainer.cpp \
    Containers/ClassTableCellContainer/tablecellcontainer.cpp
