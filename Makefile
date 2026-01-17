# make a proper makefile with different targets and tests

CC=gcc

c-copy:
	$(CC) ./include/c-copy-headers.h ./src/jobqueue.c ./src/fileutils.c ./src/c-copy.c -o c-copy.exe