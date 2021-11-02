CC = gcc --std=gnu99 -g
exe_file = smallsh
$(exe_file): main.o input.o
	$(CC) main.o input.o -o $(exe_file)

input.o: input.c input.h
	$(CC) -c input.c
main.o: main.c main.h
	$(CC) -c main.c

clean:
	rm *.o $(exe_file)

tar:
	tar -cvf $(exe_file).tar *.c *.h Makefile