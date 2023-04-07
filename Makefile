CFLAGS=-Wall -g

.PHONY: clean all

all: quantik

clean: 
	$(RM) *.o quantik

main.o: main.c quantik.h

quantik.o: quantik.c quantik.h

quantik: main.o quantik.o
