TEMPLATE = app

QT += qml quick sql

INCLUDEPATH+=.

QMAKE_CXXFLAGS_DEBUG=-O0 -g

SOURCES += main.cpp \
    Language.cpp \
    Parser.cpp \
    Tag.cpp \
    Word.cpp \
    Words.cpp \
    Examination.cpp \
    LanguageBase.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    Database.h \
    Language.h \
    Parser.h \
    Word.h \
    Words.h \
    Tag.h \
    TypesDefinitions.h \
    fWord.h \
    Examination.h \
    LanguageBase.h

OTHER_FILES += \
    langcards.ard

# Parc defines

PARC    = parc

ARD_SOURCES = $${PWD}/langcards.ard

gendbclasses.input     = ARD_SOURCES
gendbclasses.output    = Database.cpp
gendbclasses.commands  = $${PARC} qtcpp sqlite $${ARD_SOURCES} $${OUT_PWD}/
# gendbclasses.depends   = $${PARC}
gendbclasses.variable_out = SOURCES

QMAKE_EXTRA_COMPILERS += gendbclasses
PRE_TARGETDEPS += Database.cpp
