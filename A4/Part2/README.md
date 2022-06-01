### Problem 3 - Client-Server File transfer
Design a file transfer application based on client-server architecture which supports multiple clients
simultaneously. Implement it over one PC utilizing two or more VMs (Virtual Machines). Let one VM be
the server. This VM will have all the files that can be transferred to clients. Other VM’s will act as client
and connect with server and request files.

## Functionalities:
- Client sends file name to the server which checks its local disk for the file. If found it will
send the file to the requesting client.
- Clients can ask the server for its usage details (list of files client has downloaded so far,
size of data transferred etc). Usage details are client specific.
- Different kinds of errors/messages should be reported to the client. Eg: If a file is
transferred successfully then print the same on the console of the client. Similarly, do the
same for failed transfers, or files not found. Report such errors to the client.
- Think how you will differentiate between different types of messages (request for file,
file not found,request for usage details etc).
- Application should have proper termination criteria (CTRL-C should not be the only way
to make it stop). Don’t forget to close the ports before terminating the application

## Running the file
1. Run command `make`. This will create a server and client exe file.
2. Now run the server exe file `./server` on one machine
3. Run `./client <IP Address of another VM>` on another VM. 
4. For requesting File from client run `reqFile <filename>`
5. For getting the usage details run `usage-details`.

We have set limit upto 200. So if we try to send data greater than 200 i.e 8*N + 4*N >200 then data get corrupted and hence give error.
