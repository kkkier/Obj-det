######################################################################
# Automatically generated by qmake (1.06c) Thu Jun 3 15:42:26 2004
######################################################################

TEMPLATE = lib
TARGET = Histogram2
VERSION = 2.1.2

# CONFIG += debug
CONFIG += release
QMAKE_CXXFLAGS_RELEASE = -O3 #-march=pentium4 -mfpmath=sse -mmmx

CODE = $(HOME)/code

INCLUDEPATH += . $(HOME)/code/include

# Input
HEADERS += histogram.hh \
           visualhistogram.hh

SOURCES += histogram.cc \
           visualhistogram.cc


# make install
target.path = ~/code/lib/i686
headers.path = ~/code/include
headers.files = $${HEADERS}
INSTALLS += target headers

#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
QT +=  
