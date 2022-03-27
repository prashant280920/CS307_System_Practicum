## Problem -2 Linux Processes (10 marks)

Create four children C1 to C4, each having a pipe to communicate with
the parent, where :
1. C1 reads from keyboard and encrypt it using a mapping table and send it to
Parent (eg A converted to F, G to I etc). It reads the mapping table from
a predefined file.
2. C2 reads data from a text file and speaks it using “espeak” linux utility.
3. C3 copies a given input file. Here do not use the linux “cp” utility. Code
the copy function.
4. C4 does CPU monitoring (total %CPU used etc) and periodically sends stats
to the parent.
The parent reads from the four pipes and prints on the display.

## Running the file
1. Run command `make`. This will create exe files named `q2`.
2. Now for running the exe file. Exe file takes 4 arrugument. `./q2 <mapping.txt filename> <espeak.txt filename> <filename_from_copy> <filename_to_copy>`
    -  mapping.txt filename - passing a mapping txt filename 
    - espeak.txt filename - passing the txt file that contain text which will speak by the linux utility "espeak".
    - filename_from_copy - txt file ehich u want to copy
    - filename_to_copy - enter the name of the file in which u want to copy

    We provided the mapping_table.txt, espeak.txt
    Eaxmple for ruuning the exe file: ` ./q2 mapping_table.txt espeak.txt mapping_table.txt copy.txt `.