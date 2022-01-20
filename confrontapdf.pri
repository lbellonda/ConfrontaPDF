# part of ConfrontaPDF project

HEADERS	     += mainwindow.hpp \
    startupparameters.h \
    utils.h \
    status.h \
    compareresults.h \
    docinfo.h \
    batchcompare.h \
    commandlinemanager.h
SOURCES      += mainwindow.cpp \
    startupparameters.cpp \
    utils.cpp \
    status.cpp \
    compareresults.cpp \
    docinfo.cpp \
    batchcompare.cpp \
    commandlinemanager.cpp
HEADERS	     += textitem.hpp
SOURCES	     += textitem.cpp
HEADERS	     += aboutform.hpp
SOURCES      += aboutform.cpp
HEADERS	     += optionsform.hpp
SOURCES      += optionsform.cpp
HEADERS	     += helpform.hpp
SOURCES      += helpform.cpp
HEADERS	     += saveform.hpp
SOURCES      += saveform.cpp
HEADERS	     += generic.hpp
SOURCES	     += generic.cpp
HEADERS	     += sequence_matcher.hpp
SOURCES      += sequence_matcher.cpp
SOURCES      += main.cpp
HEADERS	     += lineedit.hpp
SOURCES	     += lineedit.cpp
HEADERS	     += label.hpp
SOURCES	     += label.cpp
RESOURCES    += resources.qrc
TRANSLATIONS += confrontapdf_cz.ts
TRANSLATIONS += confrontapdf_fr.ts
TRANSLATIONS += confrontapdf_de.ts
TRANSLATIONS += confrontapdf_es.ts
CODECFORTR    = UTF-8

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += printsupport widgets core
    LIBS	     += -lpoppler-qt5
    DEFINES += USE_QT5
    exists($(HOME)/opt/poppler024/) {
        message(Using locally built Poppler library)
        INCLUDEPATH += $(HOME)/opt/poppler024/include/poppler/cpp
        INCLUDEPATH += $(HOME)/opt/poppler024/include/poppler/qt5
        LIBS += -Wl,-rpath -Wl,$(HOME)/opt/poppler024/lib -L$(HOME)/opt/poppler024/lib
    } else {
        exists(/poppler_lib) {
            message(Using locally built Poppler library on Windows)
            INCLUDEPATH += /c/poppler_lib/include/poppler/cpp
            INCLUDEPATH += /c/poppler_lib/include/poppler/qt5
            LIBS += -Wl,-rpath -Wl,/c/poppler_lib/bin -Wl,-L/c/poppler_lib/bin
        } else {
            exists(/usr/include/poppler/qt5) {
                INCLUDEPATH += /usr/include/poppler/cpp
                INCLUDEPATH += /usr/include/poppler/qt5
            } else {
                exists(../poppler-win/poppler/include) {
                    message(Using locally built Poppler 5 library on ../poppler-win/poppler/include)
                    INCLUDEPATH += ../poppler-win/poppler/include/poppler/cpp
                    INCLUDEPATH += ../poppler-win/poppler/include/poppler/qt5
                    LIBS += -Wl,-rpath -Wl,../poppler-win/poppler/lib -Wl,-L../poppler-win/poppler/lib
                } else {
                    message(Using default 5)
                    INCLUDEPATH += /usr/local/include/poppler/cpp
                    INCLUDEPATH += /usr/local/include/poppler/qt5
                }
            }
        }
    }
} else {
    LIBS	     += -lpoppler-qt4
    exists($(HOME)/opt/poppler024/) {
        message(Using locally built Poppler library)
        INCLUDEPATH += $(HOME)/opt/poppler024/include/poppler/cpp
        INCLUDEPATH += $(HOME)/opt/poppler024/include/poppler/qt4
        LIBS += -Wl,-rpath -Wl,$(HOME)/opt/poppler024/lib -L$(HOME)/opt/poppler024/lib
    } else {
        exists(/poppler_lib) {
            message(Using locally built Poppler library on Windows)
            INCLUDEPATH += /c/poppler_lib/include/poppler/cpp
            INCLUDEPATH += /c/poppler_lib/include/poppler/qt4
            LIBS += -Wl,-rpath -Wl,/c/poppler_lib/bin -Wl,-L/c/poppler_lib/bin
        } else {
            exists(/usr/include/poppler/qt4) {
                INCLUDEPATH += /usr/include/poppler/cpp
                INCLUDEPATH += /usr/include/poppler/qt4
            } else {
                exists(../poppler-win/poppler/include) {
                    message(Using locally built Poppler 4 library on ../poppler-win/include)
                    INCLUDEPATH += ../poppler-win/poppler/include/poppler/cpp
                    INCLUDEPATH += ../poppler-win/poppler/include/poppler/qt4
                    LIBS += -Wl,-rpath -Wl,../poppler-win/poppler/lib -Wl,-L../poppler-win/poppler/lib
                } else {
                    message(Using default 4)
                    INCLUDEPATH += /usr/local/include/poppler/cpp
                    INCLUDEPATH += /usr/local/include/poppler/qt4
                }
            }
        }
    }
}

win32:RC_FILE = win.rc

#exists($(HOME)/opt/poppler024/) {
#    message(Using locally built Poppler library)
#    INCLUDEPATH += $(HOME)/opt/poppler024/include/poppler/cpp
#    INCLUDEPATH += $(HOME)/opt/poppler024/include/poppler/qt4
#    LIBS += -Wl,-rpath -Wl,$(HOME)/opt/poppler024/lib -L$(HOME)/opt/poppler024/lib
#} else {
#    exists(/poppler_lib) {
#	message(Using locally built Poppler library on Windows)
#	INCLUDEPATH += /c/poppler_lib/include/poppler/cpp
#	INCLUDEPATH += /c/poppler_lib/include/poppler/qt4
#	LIBS += -Wl,-rpath -Wl,/c/poppler_lib/bin -Wl,-L/c/poppler_lib/bin
#    } else {
#	exists(/usr/include/poppler/qt4) {
#	    INCLUDEPATH += /usr/include/poppler/cpp
#	    INCLUDEPATH += /usr/include/poppler/qt4
#	} else {
#           exists(../../../poppler-win/poppler/include) {
#                message(Using locally built Poppler library on ../../poppler-win/include)
#                INCLUDEPATH += ../../../poppler-win/poppler/include/poppler/cpp
#                INCLUDEPATH += ../../../poppler-win/poppler/include/poppler/qt5
#                LIBS += -Wl,-rpath -Wl,../../../poppler-win/poppler/bin -Wl,-L../../../poppler-win/bin
#            } else {
#                message(Using default)
#                INCLUDEPATH += /usr/local/include/poppler/cpp
#                INCLUDEPATH += /usr/local/include/poppler/qt4
#            }
#	}
#    }
#}
#exists($(HOME)/opt/podofo09/) {
#    message(Using locally built PoDoFo library)
#    INCLUDEPATH += $(HOME)/opt/podofo09/include/poppler/cpp
#    INCLUDEPATH += $(HOME)/opt/podofo09/include/poppler/qt4
#    LIBS += -Wl,-rpath -Wl,$(HOME)/opt/podofo09/lib64 -Wl,-L$(HOME)/opt/podofo09/lib64
#} else {
#    exists(/usr/include/podofo) {
#	INCLUDEPATH += /usr/include/podofo
#    } else {
#	INCLUDEPATH += /usr/local/include/podofo
#    }
#}


QMAKE_CXXFLAGS +=-Werror
QMAKE_CXXFLAGS +=-Wall




