default: libvimcurl.o
	gcc -shared -lc -lcurl -o libvimcurl.so libvimcurl.o

libvimcurl.o: vimcurl.c
	gcc -c -fpic vimcurl.c -o libvimcurl.o

clean:
	rm -f libvimcurl.o libvimcurl.so
