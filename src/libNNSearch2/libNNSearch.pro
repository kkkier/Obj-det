######################################################################
# Automatically generated by qmake (1.07a) Fri Nov 5 10:49:29 2004
######################################################################

TEMPLATE = lib
TARGET = NNSearch2
CONFIG += release
# CONFIG += debug
VERSION = 1.7.0

QMAKE_CXXFLAGS_RELEASE = -O3 #-march=pentium4 -mfpmath=sse -mmmx

CODE = $(HOME)/code

INCLUDEPATH += . $${CODE}/include

# Input
HEADERS += nnsearch.hh \
           balltree.hh #\
           #kballtree.hh
SOURCES += nnsearch.cc \
           balltree.cc #\
           #kballtree.cc

# make install
target.path = $${CODE}/lib/i686
headers.path = $${CODE}/include
headers.files = $${HEADERS}
INSTALLS += target headers


