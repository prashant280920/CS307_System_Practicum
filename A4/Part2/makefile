#compiler
COMPILER=gcc
flags=-c 

#File Variable 
# main=shell.cpp

all: server client

server: server.o
	$(COMPILER) $< -o $@ -lpthread

server.o: server.c
	$(COMPILER) $(flags) $<

client: ./client_folder/client.c 
	$(COMPILER) ./client_folder/client.c -o ./client_folder/client

# client.o: client.c  
# 	$(COMPILER) $(flags) ./client_folder/client.c

clean:
	rm -rf *o server ./client_folder/client ./client_folder/o* 