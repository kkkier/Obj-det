######################################################################
# Automatically generated by qmake (1.06c) Thu Jun 3 15:42:26 2004
######################################################################

TEMPLATE = lib
TARGET = image
VERSION = 2.0.0

# CONFIG += debug
CONFIG += release
QMAKE_CXXFLAGS_RELEASE = -O3 -DQT_THREAD_SUPPORT #-march=pentium4 -mfpmath=sse -mmmx

CODE = $(HOME)/code

INCLUDEPATH += . $(HOME)/code/include

# Input
HEADERS += image.h \
           hsiimage.h \
           rgbimage.h

SOURCES += hsiimage.cc \
           rgbimage.cc


# make install
target.path = $${CODE}/lib/i686
headers.path = $${CODE}/include
headers.files = $${HEADERS}
INSTALLS += target headers

