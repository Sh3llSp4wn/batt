CC=clang++
CFLAGS=-I/usr/local/include/ -ggdb -Wall
CFLAGS=-I/usr/local/include/ -Os -Wall

all:
	$(CC) $(CFLAGS) -o batt batt.cpp /usr/local/lib/libboost_json.a

install:
	install -s batt /usr/local/bin/batt

clean:
	rm -f batt
