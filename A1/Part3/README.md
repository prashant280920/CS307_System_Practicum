### Problem 3 - Matrix Multiplication

### General Details
1. Programming Language - C++
2. Module Used - iostream, thread, semaphore.h, mutex, condition_variable, vector, chrono, unistd.h, unordered_map
3. gcc version - g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0

## About the codes
1. The part3 is matrix multiplication which was done by two method
2. first method is simple sequential approach
3. Second method is using threading.
4. To change the number of threads in program, go to line number 115 and change the value given in function MatMulThread();
5. The thread number given to this number should be such that (size of matrix/thread number) gives a integer.


## Running the file
1. Run command `make`. This will create exe files named `q3`.
2. Now run the exe file by using `./q3` followed by size of the matrix i.e., `./q3 size_of_matrix`. 
3. If you want to clean all objects file (.o file) and exe file. Then just run `make clean`.
