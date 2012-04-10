CC=gcc
PROGOPTS=-DCMEM
DBGFLAG=-ggdb 
OPTFLAG=-O0
CFLAGS=-std=c99 $(DBGFLAG) -Wall $(OPTFLAG) $(PROGOPTS)
LDLIBS=
LDFLAGS=$(DBGFLAG) $(OPTFLAG)

SOURCES=router.c parse.c trie.c test.c

OBJECTS=$(SOURCES:.c=.o)

TARGET=router


.PHONY : clean 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(LDFLAGS) $(LDLIBS) $(OBJECTS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.depend: depend 

test: $(TARGET)
	./$(TARGET) | circo -Tpng | display


depend: $(SOURCES)
	ctags -R
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;
include .depend

clean:
	rm $(OBJECTS)
