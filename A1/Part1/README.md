### Problem 1 - Creating a myshell
We have created myshell.

### General Details
1. Programming Language - C++
2. Module Used - iostream, fstream, map, vector, string, dirent.h, sys/wait.h, sys/types.h, unistd.h, stdlib.h, stdio.h
3. gcc version - g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0

## Commands that the myshell support
1. clr
2. pause
3. help
4. quit/Ctrl+D
5. history
6. cd 'directory'
7. dir 'directory'
8. environ
9. echo 'comment'
10. The shell able to take its command line input from a file. That is, if the command line is invoked with command line argument: **myshell "batchfile"** then "batchfile" is assumed to contain a set of command lines for the shell to process. When the end of file is reached, the shell should exit.

## Running the file
1. Run command `make`. This will create a <myshell> exe file.
2. Now run the exe file `./myshell`. Now you can run the shell commands 
3. If you want to run batchfile. I have given a batchfile for testing. Just run `./myshell batchfile`. 
4. If you want to clean all objects file (.o file) and exe file. Then just run `make clean`.
