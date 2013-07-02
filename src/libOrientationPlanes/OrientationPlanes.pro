######################################################################
# Automatically generated by qmake (1.06c) Thu Jun 3 15:42:26 2004
######################################################################

TEMPLATE = lib
TARGET = OrientationPlanes
#CONFIG += debug
CONFIG += release

INCLUDEPATH += $(HOME)/code/include

CODE = $(HOME)/code

# Input
HEADERS += convolve.hh \
           fft.h \
           gaussfilterbank.hh \
           gausskernel.hh \
           hilbert.h \
           orientationenergy.hh \
           rotate.hh \
           utils.h
SOURCES += convolve.cc \
           fft.cpp \
           gaussfilterbank.cc \
           gausskernel.cc \
           hilbert.cpp \
           orientationenergy.cc \
           rotate.cc \
           utils.cpp

# make install
target.path = $${CODE}/lib/i686
headers.path = $${CODE}/include
headers.files = gaussfilterbank.hh gausskernel.hh orientationenergy.hh

INSTALLS += target headers

