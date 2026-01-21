# c-copy
custom copy command created in c


# application usage

```
./c-copy <source_dir> <dest_dir> [<threadCount>]
```

all files and directories within source_dir will be copied to dest_dir.

while wall-clock time usually depends more on I/O wait than CPU time, the program does support multi-threaded copying. the default thread count is 4.

a live progress percentage is displayed on the command line, and the user will be notified if some files could not be copied.

# building

the included makefile allows for easy building, testing and cleanup.
```Makefile
make all
make test
make clean
```
