
#CC=m68k-apple-macos-gcc
#CXX=m68k-apple-macos-g++
#CC=powerpc-apple-macos-gcc
#CXX=powerpc-apple-macos-g++

CFLAGS= -I/opt/local/include -I/opt/local/include/GL
LDFLAGS= -lglut -lglu -lgl -lm -L/opt/local/lib

OBJS=$(subst .c,.o,$(wildcard *.c))

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
#	$(CC) -o atlantis.code.bin $(OBJS) $(LDFLAGS)
	$(CC) -o atlantis $(OBJS) $(LDFLAGS)
#	../xcoff2app.sh atlantis

clean:
	rm -f *.o *.bin *.pef *.APPL *.dsk *.gdb atlantis
