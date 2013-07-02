######################################################################
# Automatically generated by qmake (1.06c) Thu Jun 3 15:42:26 2004
######################################################################

TEMPLATE = lib
TARGET = Matrix
VERSION = 2.0.0

#CONFIG += debug
CONFIG += release
QMAKE_CXXFLAGS_RELEASE = -O3 -DQT_THREAD_SUPPORT #-march=pentium4 -mfpmath=sse -mmmx

CODE = $(HOME)/code

INCLUDEPATH += . $(HOME)/code/include

# Input
HEADERS += featurematrix.hh illegaloperationexception.hh \
           nr.h nrutil.h

SOURCES += featurematrix.cc \
           lubksb.c ludcmp.c nrutil.c

FEATURE_LIBS = -lFeatures
STD_LIBS     = -lm -lstdc++
LIBS += -L$${CODE}/lib/i686 $${STD_LIBS}


# make install
target.path = $${CODE}/lib/i686
headers.path = $${CODE}/include
headers.files = $${HEADERS}
INSTALLS += target headers

