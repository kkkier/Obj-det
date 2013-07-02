TEMPLATE = lib
TARGET = Container
CONFIG += release
# CONFIG += debug
VERSION = 1.0.0

QMAKE_CXXFLAGS_RELEASE = -O3 #-march=pentium4 -mfpmath=sse -mmmx

CODE = $(HOME)/code

INCLUDEPATH += . $${CODE}/include

# Input
HEADERS += container.hh \
           sharedcontainer.hh 

#SOURCES += container.cc

# make install
target.path = $${CODE}/lib/i686
headers.path = $${CODE}/include
headers.files = $${HEADERS}
INSTALLS += target headers

