
all:
	make -C glu -f Makefile.r68ppc
	make -C GLUT\ 3.7 -f Makefile.r68ppc
	make -i -C MUI -f Makefile.r68ppc
	cp */*.a .

install:
	make -f Makefile.r68ppc
	cp *.a $(HOME)/Retro68-build/toolchain/powerpc-apple-macos/lib/.

clean:
	make -C glu -f Makefile.r68ppc clean
	make -C GLUT\ 3.7 -f Makefile.r68ppc clean
	make -C MUI -f Makefile.r68ppc clean
	rm -f *.a