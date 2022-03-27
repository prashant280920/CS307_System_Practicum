## Problem -2 Linux Processes (10 marks)

Create four children C1 to C4, each having a pipe to communicate with
the parent, where :
[i] C1 reads from keyboard and encrypt it using a mapping table and send it to
Parent (eg A converted to F, G to I etc). It reads the mapping table from
a predefined file.
[ii] C2 reads data from a text file and speaks it using “espeak” linux utility.
[iii] C3 copies a given input file. Here do not use the linux “cp” utility. Code
the copy function.
[iv] C4 does CPU monitoring (total %CPU used etc) and periodically sends stats
to the parent.
The parent reads from the four pipes and prints on the display.

### Running the file
1. Run command `make`. This will create exe files named `q2`.
2. Now run the exe file by using `./q3` followed by size of the matrix i.e., `./q3 size_of_matrix`.