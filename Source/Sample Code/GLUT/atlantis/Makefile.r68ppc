
#CC=m68k-apple-macos-gcc
#CXX=m68k-apple-macos-g++
CC=powerpc-apple-macos-gcc
CXX=powerpc-apple-macos-g++

CFLAGS= -I../../../../Headers
LDFLAGS=-lOpenGLUtility -lOpenGLLibrary -lglut -lDrawSprocketLib -lContextualMenu -lm

OBJS=$(subst .c,.o,$(wildcard *.c))

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
#	$(CC) -o atlantis.code.bin $(OBJS) $(LDFLAGS)
	$(CC) -o atlantis $(OBJS) $(LDFLAGS)
	$(HOME)/sdl2macos9/xcoff2app.sh atlantis

clean:
	rm -f *.o *.bin *.pef *.APPL *.dsk *.gdb atlantis
