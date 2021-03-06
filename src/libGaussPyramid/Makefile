#############################################################################
# Makefile for building: libGaussPyramid.so.2.0.2
# Generated by qmake (1.07a) (Qt 3.3.8b) on: Tue Dec 25 06:29:07 2012
# Project:  libGaussPyramid.pro
# Template: lib
# Command: $(QMAKE) -o Makefile libGaussPyramid.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -Wall -W -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic -fno-strict-aliasing -fPIC  -DQT_NO_DEBUG -DQT_SHARED -DQT_TABLET_SUPPORT -DQT_THREAD_SUPPORT
CXXFLAGS = -pipe -Wall -W -O3 -fPIC  -DQT_NO_DEBUG -DQT_SHARED -DQT_TABLET_SUPPORT -DQT_THREAD_SUPPORT
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/usr/lib64/qt-3.3/mkspecs/default -I. -I. -I$(HOME)/code/include -I$(QTDIR)/include
LINK     = g++
LFLAGS   = -shared -Wl,-soname,libGaussPyramid.so.2
LIBS     = $(SUBLIBS) -L$(QTDIR)/lib -lqt-mt
AR       = ar cqs
RANLIB   = 
MOC      = $(QTDIR)/bin/moc
UIC      = $(QTDIR)/bin/uic
QMAKE    = qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = ./

####### Files

HEADERS = pyramiddata.hh \
		polcoe.h \
		gausspyramid.hh \
		laplacepyramid.hh
SOURCES = polcoe.cc \
		pyramiddata.cc \
		gausspyramid.cc \
		laplacepyramid.cc
OBJECTS = polcoe.o \
		pyramiddata.o \
		gausspyramid.o \
		laplacepyramid.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = 
OBJMOC = 
DIST	   = libGaussPyramid.pro
QMAKE_TARGET = GaussPyramid
DESTDIR  = 
TARGET   = libGaussPyramid.so.2.0.2
TARGETA	= libGaussPyramid.a
TARGETD	= libGaussPyramid.so.2.0.2
TARGET0	= libGaussPyramid.so
TARGET1	= libGaussPyramid.so.2
TARGET2	= libGaussPyramid.so.2.0

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile  $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC) $(SUBLIBS) $(OBJCOMP)  
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS) $(OBJCOMP)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)



staticlib: $(TARGETA)

$(TARGETA):  $(UICDECLS) $(OBJECTS) $(OBJMOC) $(OBJCOMP)
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS) $(OBJMOC)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd $(QTDIR)/src/moc && $(MAKE) )

Makefile: libGaussPyramid.pro  /usr/lib64/qt-3.3/mkspecs/default/qmake.conf /usr/lib64/qt-3.3/lib/libqt-mt.prl
	$(QMAKE) -o Makefile libGaussPyramid.pro
qmake: 
	@$(QMAKE) -o Makefile libGaussPyramid.pro

dist: 
	@mkdir -p .tmp/GaussPyramid && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .tmp/GaussPyramid/ && ( cd `dirname .tmp/GaussPyramid` && $(TAR) GaussPyramid.tar GaussPyramid && $(GZIP) GaussPyramid.tar ) && $(MOVE) `dirname .tmp/GaussPyramid`/GaussPyramid.tar.gz . && $(DEL_FILE) -r .tmp/GaussPyramid

mocclean:

uiclean:

yaccclean:
lexclean:
clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)
	-$(DEL_FILE) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA)


FORCE:

####### Compile

polcoe.o: polcoe.cc 

pyramiddata.o: pyramiddata.cc pyramiddata.hh

gausspyramid.o: gausspyramid.cc gausspyramid.hh \
		pyramiddata.hh

laplacepyramid.o: laplacepyramid.cc laplacepyramid.hh \
		polcoe.h \
		pyramiddata.hh \
		gausspyramid.hh

####### Install

install_target: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)$(HOME)/code/lib/i686/" || $(MKDIR) "$(INSTALL_ROOT)$(HOME)/code/lib/i686/"
	-$(INSTALL_FILE) "$(TARGET)" "$(INSTALL_ROOT)$(HOME)/code/lib/i686/$(TARGET)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)$(HOME)/code/lib/i686/$(TARGET0)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)$(HOME)/code/lib/i686/$(TARGET1)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)$(HOME)/code/lib/i686/$(TARGET2)"

uninstall_target: 
	-$(DEL_FILE) "$(INSTALL_ROOT)$(HOME)/code/lib/i686/$(TARGET)"
	-$(DEL_FILE) "$(INSTALL_ROOT)$(HOME)/code/lib/i686/$(TARGET0)"
	-$(DEL_FILE) "$(INSTALL_ROOT)$(HOME)/code/lib/i686/$(TARGET1)"
	-$(DEL_FILE) "$(INSTALL_ROOT)$(HOME)/code/lib/i686/$(TARGET2)"
	-$(DEL_DIR) "$(INSTALL_ROOT)$(HOME)/code/lib/i686/"


install_headers: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)$(HOME)/code/include/" || $(MKDIR) "$(INSTALL_ROOT)$(HOME)/code/include/"
	-$(INSTALL_FILE) "pyramiddata.hh" "$(INSTALL_ROOT)$(HOME)/code/include/"
	-$(INSTALL_FILE) "polcoe.h" "$(INSTALL_ROOT)$(HOME)/code/include/"
	-$(INSTALL_FILE) "gausspyramid.hh" "$(INSTALL_ROOT)$(HOME)/code/include/"
	-$(INSTALL_FILE) "laplacepyramid.hh" "$(INSTALL_ROOT)$(HOME)/code/include/"


uninstall_headers: 
	-$(DEL_FILE) -r "$(INSTALL_ROOT)$(HOME)/code/include/pyramiddata.hh"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)$(HOME)/code/include/polcoe.h"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)$(HOME)/code/include/gausspyramid.hh"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)$(HOME)/code/include/laplacepyramid.hh"
	-$(DEL_DIR) "$(INSTALL_ROOT)$(HOME)/code/include/"


install: install_target install_headers  

uninstall: uninstall_target uninstall_headers  

