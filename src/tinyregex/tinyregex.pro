TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tinyregex.cpp \
    astparser.cpp \
    message.cpp \
    automaton.cpp \
    nfagraph.cpp \
    regexresult.cpp


HEADERS += \
    tinyregex.h \
    base.h \
    astnode.h \
    astparser.h \
    message.h \
    automaton.h \
    nfagraph.h \
    regexresult.h


DISTFILES += \

