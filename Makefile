#############################################################################
# Makefile for building: parser
# Generated by qmake (2.01a) (Qt 4.6.2) on: L feb. 22 22:30:16 2010
# Project:  parser.pro
# Template: app
# Command: /usr/bin/qmake -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile parser.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4 -I.
LINK          = g++
LFLAGS        = -Wl,-rpath,/usr/lib64/qt4
LIBS          = $(SUBLIBS)  -L/usr/lib64/qt4 -lQtCore -L/usr/lib64 -L/usr/lib64/qt4 -lgthread-2.0 -lrt -lglib-2.0 -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		rrdtool.cpp \
		parse.cpp \
		parseiostat.cpp \
		parsedfstat.cpp \
		parsefanstat.cpp \
		parseprtpicl.cpp \
		parsesarstat.cpp 
OBJECTS       = main.o \
		rrdtool.o \
		parse.o \
		parseiostat.o \
		parsedfstat.o \
		parsefanstat.o \
		parseprtpicl.o \
		parsesarstat.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		parser.pro
QMAKE_TARGET  = parser
DESTDIR       = 
TARGET        = parser

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: parser.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib64/qt4/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile parser.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib64/qt4/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile parser.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/parser1.0.0 || $(MKDIR) .tmp/parser1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/parser1.0.0/ && $(COPY_FILE) --parents rrdtool.h parse.h parseiostat.h parsedfstat.h parsefanstat.h parseprtpicl.h parsesarstat.h .tmp/parser1.0.0/ && $(COPY_FILE) --parents main.cpp rrdtool.cpp parse.cpp parseiostat.cpp parsedfstat.cpp parsefanstat.cpp parseprtpicl.cpp parsesarstat.cpp .tmp/parser1.0.0/ && (cd `dirname .tmp/parser1.0.0` && $(TAR) parser1.0.0.tar parser1.0.0 && $(COMPRESS) parser1.0.0.tar) && $(MOVE) `dirname .tmp/parser1.0.0`/parser1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/parser1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: main.cpp parseiostat.h \
		parse.h \
		rrdtool.h \
		parsedfstat.h \
		parsesarstat.h \
		parsefanstat.h \
		parseprtpicl.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

rrdtool.o: rrdtool.cpp rrdtool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o rrdtool.o rrdtool.cpp

parse.o: parse.cpp parse.h \
		rrdtool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parse.o parse.cpp

parseiostat.o: parseiostat.cpp parseiostat.h \
		parse.h \
		rrdtool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parseiostat.o parseiostat.cpp

parsedfstat.o: parsedfstat.cpp parsedfstat.h \
		parse.h \
		rrdtool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parsedfstat.o parsedfstat.cpp

parsefanstat.o: parsefanstat.cpp parsefanstat.h \
		parseprtpicl.h \
		parse.h \
		rrdtool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parsefanstat.o parsefanstat.cpp

parseprtpicl.o: parseprtpicl.cpp parseprtpicl.h \
		parse.h \
		rrdtool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parseprtpicl.o parseprtpicl.cpp

parsesarstat.o: parsesarstat.cpp parsesarstat.h \
		parse.h \
		rrdtool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parsesarstat.o parsesarstat.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

