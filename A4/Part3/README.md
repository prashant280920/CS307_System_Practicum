### Problem 3 - Error Control
Error Control is important as it ensures that data transferred over the network doesn't get corrupted. In this
task, you have to perform error detection. Error detection is a challenging task because the receiver has to
decide if received data is correct or not, without having a copy of the original message. Burst errors are
errors in which 2 or more bits in the data unit have changed. Cyclic Redundancy Check(CRC) is an
algorithm that is used to detect such errors.

## Functionalities:
- In this task, use a single pair of server and client.
- Instead of actual bits, generate strings consisting of ‘0’ and ‘1’. Apply CRC algorithm on
these strings.
- Agree upon any divisor of length 5(eg 11001).
- Generate a message of length 8*N in server. Apply CRC on it and send it to the client.
- Apply the algorithm on each 8 length block of the original message.
- On the client detect if there is any error or not

## Running the file
1. Run command `make`. This will create a server and client exe file.
2. Now run the server exe file `./server` on one machine
3. Run `./client <IP Address of another VM>` on another VM. 
4. In client enter value of 'N' which will generate random binary string of 8*N.

We have set limit upto 200. So if we try to send data greater than 200 i.e 8*N + 4*N >200 then data get corrupted and hence give error.
