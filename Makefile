CC=gcc
CFLAGS=-g -w

url_extend: 
	$(CC) $(CFLAGS) -o url_extend url_extend.c

clean:
	rm -f url_extend
