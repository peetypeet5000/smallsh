CC = gcc --std=gnu99 -g
exe_file = movies_by_year
$(exe_file): main.o file_parser.o interface.o
	$(CC) main.o file_parser.o interface.o -o $(exe_file)

interface.o: interface.c interface.h
	$(CC) -c interface.c
file_parser.o: file_parser.c file_parser.h
	$(CC) -c file_parser.c
main.o: main.c main.h
	$(CC) -c main.c

clean:
	rm *.o $(exe_file)

tar:
	tar -cvf $(exe_file).tar *.c *.h bft.txt Makefile