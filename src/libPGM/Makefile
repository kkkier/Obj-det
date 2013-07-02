CC=g++
INCDIR=/root/code/include
LIBDIR=/root/code/lib/i686
QTDIR=/usr/local/Trolltech/Qt-4.8.4/include/QtGui
CFLAGS=-c -Wall -g -I/root/code/bin/vlfeat-0.9.16/vl -I$(INCDIR) -I$(QTDIR) -I.
LDFLAGS=-L$(LIBDIR) -L/usr/local/Trolltech/Qt-4.8.4/lib -lvl -lpthread -lm -lQtGui
SOURCES=mypgm.cc 
OBJECTS=mypgm.o 
OUTPUT=libpgm.so

all: $(SOURCES) $(OUTPUT) test

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT): $(SOURCES)
	gcc $(CFLAGS) $(SOURCES) $(LDFLAGS) -lstdc++ -fPIC -shared -o $@

test: test.cc $(OUTPUT)
	$(CC) $(CFLAGS) test.cc 
	gcc -L. -lpgm $(LDFLAGS) -lstdc++ -o test test.o

install:
	cp mypgm.hh $(INCDIR)
	cp libpgm.so $(LIBDIR)

clean:
	-rm -f *.o libpgm.so test 
