CC=g++
INCDIR=/root/code/include
QTDIR=/usr/local/Trolltech/Qt-4.8.4/include/Qt
LIBDIR=/root/code/lib/i686
CFLAGS=-c -g -Wall -I$(INCDIR) -I$(QTDIR)  -I. 
LDFLAGS=-L. -L$(LIBDIR) -lpgm -lm -lvl -lpthread -lQtGui
SOURCES=siftdetector.cc 
OBJECTS=$(SOURCES:.cc=.o) 
OUTPUT=libsift.so
TESTSOURCES=test.cc
TESTOBJ=test.o
TEST=test

all: $(SOURCES) $(OUTPUT) $(TEST)

$(OUTPUT): $(OBJECTS)
	gcc $(LDFLAGS) -lstdc++ -shared -fPIC  -o $@ $(OBJECTS)

.cc.o:
	$(CC) $(CFLAGS) $< -o $@ -fPIC

clean:
	rm -f *.o $(OUTPUT) $(TEST)

install:
	cp siftdetector.hh $(INCDIR)
	cp libsift.so $(LIBDIR)

$(TEST): $(TESTSOURCES)
	$(CC) $(CFLAGS) $(TESTSOURCES)
	$(CC) $(LDFLAGS) -lsift -o $@ $(TESTOBJ)

