#--------------------------------------------------------
#
# Pro file for Windows builds with Qt Creator
#
#--------------------------------------------------------

QT += core

TEMPLATE = lib
TARGET = dsdcc

CONFIG(MINGW32):LIBDSDCCSRC = "D:\softs\dsdcc"
CONFIG(MINGW64):LIBDSDCCSRC = "D:\softs\dsdcc"

CONFIG(MINGW32):LIBMBELIBSRC = "D:\softs\mbelib"
CONFIG(MINGW64):LIBMBELIBSRC = "D:\softs\mbelib"

INCLUDEPATH += $$LIBDSDCCSRC
INCLUDEPATH += $$LIBMBELIBSRC

DEFINES += DSD_USE_MBELIB=1

CONFIG(Release):build_subdir = release
CONFIG(Debug):build_subdir = debug

SOURCES = $$LIBDSDCCSRC/descramble.cpp\
$$LIBDSDCCSRC/dmr_data.cpp\
$$LIBDSDCCSRC/dmr_voice.cpp\
$$LIBDSDCCSRC/dsd_decoder.cpp\
$$LIBDSDCCSRC/dsd_filters.cpp\
$$LIBDSDCCSRC/dsd_logger.cpp\
$$LIBDSDCCSRC/dsd_mbe.cpp\
$$LIBDSDCCSRC/dsd_opts.cpp\
$$LIBDSDCCSRC/dsd_state.cpp\
$$LIBDSDCCSRC/dsd_symbol.cpp\
$$LIBDSDCCSRC/dstar.cpp\
$$LIBDSDCCSRC/p25p1_heuristics.cpp

HEADERS = $$LIBDSDCCSRC/descramble.h\
$$LIBDSDCCSRC/dmr_data.h\
$$LIBDSDCCSRC/dmr_voice.h\
$$LIBDSDCCSRC/dsd_decoder.h\
$$LIBDSDCCSRC/dsd_filters.h\
$$LIBDSDCCSRC/dsd_logger.h\
$$LIBDSDCCSRC/dsd_mbe.h\
$$LIBDSDCCSRC/dsd_opts.h\
$$LIBDSDCCSRC/dsd_state.h\
$$LIBDSDCCSRC/dsd_symbol.h\
$$LIBDSDCCSRC/dstar.h\
$$LIBDSDCCSRC/p25p1_heuristics.h

LIBS += -L../mbelib/$${build_subdir} -lmbelib