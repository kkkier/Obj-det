######################################################################
# Automatically generated by qmake (1.07a) Fri Nov 5 10:49:29 2004
######################################################################

TEMPLATE = lib
TARGET = Features2
CONFIG += release
#CONFIG += debug
VERSION = 2.6.1

QMAKE_CXXFLAGS_RELEASE = -O3 #-march=pentium4 -mfpmath=sse -mmmx

CODE = $(HOME)/code

INCLUDEPATH += . $${CODE}/include

# Input
HEADERS += featurevector.hh
SOURCES += featurevector.cc

# make install
target.path = ~/code/lib/i686
headers.path = ~/code/include
headers.files = $${HEADERS}
INSTALLS += target headers


#The following line was inserted by qt3to4
QT +=  qt3support 
