#############################################################################
# Makefile for building: featExtract
# Generated by qmake (2.01a) (Qt 4.8.4) on: ?? 6? 18 10:55:02 2013
# Project:  featExtract.pro
# Template: app
# Command: /usr/local/Trolltech/Qt-4.8.4/bin/qmake -o Makefile featExtract.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe  -g-Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe  -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/local/Trolltech/Qt-4.8.4/mkspecs/linux-g++-64 -I. -I/usr/local/Trolltech/Qt-4.8.4/include/QtCore -I/usr/local/Trolltech/Qt-4.8.4/include/QtGui -I/usr/local/Trolltech/Qt-4.8.4/include/Qt3Support -I/usr/local/Trolltech/Qt-4.8.4/include -I. -I$(HOME)/code/include -I/usr/include/libxml2 -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-O1 -Wl,-rpath,/usr/local/Trolltech/Qt-4.8.4/lib
LIBS          = $(SUBLIBS)  -L/usr/local/Trolltech/Qt-4.8.4/lib -L$(HOME)/code/lib/i686 -lQtTools2 -limage2 -lGrayImage2 -lRGBImage2 -lGaussDeriv2 -lCanny2 -lMorphology2 -lScaleSpace2 -lInterestPts2 -lPatchExtraction6 -lFeatures2 -lPCA2 -lMath2 -lMatrix2 -lChiSquare2 -lNNSearch3 -lCluster3 -lHistogram2 -lEdgeSIFT2 -lvl -lpgm -lsift -lRandomForest -lxml2 -lQt3Support -L/usr/local/Trolltech/Qt-4.8.4/lib -lQtSql -lQtXml -L/usr/X11R6/lib64 -lQtNetwork -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/local/Trolltech/Qt-4.8.4/bin/qmake
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

SOURCES       = main.cc \
		kmeans.cc\
		votingspace.cc \
		featExtract.cc moc_featExtract.cpp
OBJECTS       = main.o \
		kmeans.o\
		votingspace.o \
		featExtract.o \
		moc_featExtract.o
DIST          = /usr/local/Trolltech/Qt-4.8.4/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/linux.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/gcc-base.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/gcc-base-unix.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/g++-base.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/g++-unix.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/release.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/lex.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/include_source_dir.prf \
		featExtract.pro
QMAKE_TARGET  = featExtract
DESTDIR       = 
TARGET        = featExtract

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

Makefile: featExtract.pro  /usr/local/Trolltech/Qt-4.8.4/mkspecs/linux-g++-64/qmake.conf /usr/local/Trolltech/Qt-4.8.4/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/linux.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/gcc-base.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/gcc-base-unix.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/g++-base.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/g++-unix.conf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/release.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/lex.prf \
		/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/include_source_dir.prf \
		/usr/local/Trolltech/Qt-4.8.4/lib/libQt3Support.prl \
		/usr/local/Trolltech/Qt-4.8.4/lib/libQtSql.prl \
		/usr/local/Trolltech/Qt-4.8.4/lib/libQtCore.prl \
		/usr/local/Trolltech/Qt-4.8.4/lib/libQtXml.prl \
		/usr/local/Trolltech/Qt-4.8.4/lib/libQtGui.prl \
		/usr/local/Trolltech/Qt-4.8.4/lib/libQtNetwork.prl
	$(QMAKE) -o Makefile featExtract.pro
/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/unix.conf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/linux.conf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/gcc-base.conf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/gcc-base-unix.conf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/g++-base.conf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/common/g++-unix.conf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/qconfig.pri:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/modules/qt_webkit_version.pri:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/qt_functions.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/qt_config.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/exclusive_builds.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/default_pre.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/release.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/default_post.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/warn_on.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/qt.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/unix/thread.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/moc.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/resources.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/uic.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/yacc.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/lex.prf:
/usr/local/Trolltech/Qt-4.8.4/mkspecs/features/include_source_dir.prf:
/usr/local/Trolltech/Qt-4.8.4/lib/libQt3Support.prl:
/usr/local/Trolltech/Qt-4.8.4/lib/libQtSql.prl:
/usr/local/Trolltech/Qt-4.8.4/lib/libQtCore.prl:
/usr/local/Trolltech/Qt-4.8.4/lib/libQtXml.prl:
/usr/local/Trolltech/Qt-4.8.4/lib/libQtGui.prl:
/usr/local/Trolltech/Qt-4.8.4/lib/libQtNetwork.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile featExtract.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/featExtract1.0.0 || $(MKDIR) .tmp/featExtract1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/featExtract1.0.0/ && $(COPY_FILE) --parents votingspace.hh featExtract.hh .tmp/featExtract1.0.0/ && $(COPY_FILE) --parents main.cc votingspace.cc featExtract.cc .tmp/featExtract1.0.0/ && (cd `dirname .tmp/featExtract1.0.0` && $(TAR) featExtract1.0.0.tar featExtract1.0.0 && $(COMPRESS) featExtract1.0.0.tar) && $(MOVE) `dirname .tmp/featExtract1.0.0`/featExtract1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/featExtract1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_featExtract.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_featExtract.cpp
moc_featExtract.cpp: votingspace.hh \
		featExtract.hh
	/usr/local/Trolltech/Qt-4.8.4/bin/moc $(DEFINES) $(INCPATH) featExtract.hh -o moc_featExtract.cpp

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
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: main.cc 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cc

votingspace.o: votingspace.cc votingspace.hh
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o votingspace.o votingspace.cc

featExtract.o: featExtract.cc featExtract.hh \
		votingspace.hh
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o featExtract.o featExtract.cc

moc_featExtract.o: moc_featExtract.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_featExtract.o moc_featExtract.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

