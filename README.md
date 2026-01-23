# c-copy
custom copy command created in C


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
```
```Makefile
make test
```
```Makefile
make clean
```

# misc
usage of coding assistants has been marked in code with comments. summary:
- makefile was created with significant consultation from chatgpt
- in test-headers.h, a short macro recommended by chatgpt is defined
- in a some cases, chatgpt was used to answer general C questions (not to generate specific implementations unless listed otherwise)
- in copyfile(), an ai assistant was used to identify possible failure points
- in exploredir(), an ai assistant recommended usage of snprintf for path reconstruction
