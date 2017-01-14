TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    tinyregex.cpp \
    astparser.cpp \
    automaton.cpp \
    nfagraph.cpp \
    regexresult.cpp \
    stringutilities.cpp \
    regexsearch.cpp \
    main.cpp


HEADERS += \
    tinyregex.h \
    base.h \
    astnode.h \
    astparser.h \
    automaton.h \
    nfagraph.h \
    regexresult.h \
    attribute.h \
    regexerror.h \
    stringutilities.h \
    regexsearch.h


DISTFILES += \

