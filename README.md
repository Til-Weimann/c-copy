# c-copy
custom copy command created in C


# application usage

```
./c-copy <source_dir> <dest_dir> [<thread_count>]
```

all files and directories within source_dir will be copied to dest_dir.

a live progress percentage is displayed on the command line, and the user will be notified if some files could not be copied.

the program supports multi-threaded copying, however the default thread count is 1 because multi threaded copying is not benefitial under normal circumstances:
- wall-clock time for such operations is highly I/O-bound, not CPU-bound
- most storage media are faster when reading/writing sequentially

however in some cases (e. g. use of a RAM drive), these caveats might not apply. the threading feature can also be used to demonstrate or quantify these considerations for educational purposes.

# demo

https://github.com/user-attachments/assets/8d61bd6f-e337-4e6e-af10-3b7b3e52e9e5

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
- in copyfile(), chatgpt was used to identify possible failure points
- in exploredir(), chatgpt recommended usage of snprintf for path reconstruction
