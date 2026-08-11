
#CC=m68k-apple-macos-gcc
#CXX=m68k-apple-macos-g++
CC=powerpc-apple-macos-gcc
CXX=powerpc-apple-macos-g++

CFLAGS= -I../../../../Headers
LDFLAGS=-lOpenGLLibrary -lm

OBJS=$(subst .c,.o,$(wildcard *.c))

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
#	$(CC) -o IndexIntro.code.bin $(OBJS) $(LDFLAGS)
	$(CC) -o IndexIntro $(OBJS) $(LDFLAGS)
	$(HOME)//sdl2macos9/xcoff2app.sh IndexIntro

clean:
	rm -f *.o *.bin *.pef *.APPL *.dsk *.gdb IndexIntro
