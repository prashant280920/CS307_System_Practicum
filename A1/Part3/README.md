### Problem 3 - Matrix Multiplication


## About the codes
1. The part3 is matrix multiplication which was done by two method
2. first method is simple sequential approach
3. Second method is using threading.
4. To change the number of threads in program, go to line number 122 and change the value given in function MatMulThread();



## Running the file
1. Run command `make`. This will create exe files named `q2`.
2. Now for running the exe file. Exe file takes 4 arrugument. `./q2 <mapping.txt filename> <espeak.txt filename> <filename_from_copy> <filename_to_copy>`
    -  mapping.txt filename - passing a mapping txt filename 
    - espeak.txt filename - passing the txt file that contain text which will speak by the linux utility "espeak".
    - filename_from_copy - txt file ehich u want to copy
    - filename_to_copy - enter the name of the file in which u want to copy

    We provided the mapping_table.txt, espeak.txt
    Eaxmple for ruuning the exe file: ` ./q2 mapping_table.txt espeak.txt mapping_table.txt copy.txt `.

