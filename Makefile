
all:
	make -C Source -f Makefile
	
clean:
	make -C Source -f Makefile clean
	rm -f */*.o */*.a
