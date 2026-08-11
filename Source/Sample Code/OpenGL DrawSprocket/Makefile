
#CC=m68k-apple-macos-gcc
#CXX=m68k-apple-macos-g++
CC=powerpc-apple-macos-gcc
CXX=powerpc-apple-macos-g++

CFLAGS= -I../../../Headers
LDFLAGS=-lOpenGLLibrary -lDrawSprocketLib -lm

OBJS=$(subst .c,.o,$(wildcard *.c))

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
#	$(CC) -o OpenGLDrawSprocket.code.bin $(OBJS) $(LDFLAGS)
	$(CC) -o OpenGLDrawSprocket $(OBJS) $(LDFLAGS)
	$(HOME)//sdl2macos9/xcoff2app.sh OpenGLDrawSprocket

clean:
	rm -f *.o *.bin *.pef *.APPL *.dsk *.gdb
