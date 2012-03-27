CC=gcc
DBGFLAG=-ggdb 
CFLAGS=-std=c99 $(DBGFLAG) -Wall
LDLIBS=
LDFLAGS=$(DBGFLAG) -Os

SOURCES=router.c 

OBJECTS=$(SOURCES:.c=.o)

TARGET=router


.PHONY : clean run 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(LDFLAGS) $(LDLIBS) $(OBJECTS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.depend: depend 


depend: $(SOURCES)
	ctags -R
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;
include .depend

clean:
	rm $(OBJECTS)
