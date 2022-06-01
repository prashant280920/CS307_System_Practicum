#compiler
COMPILER=gcc
flags=-c 

#File Variable 
# main=shell.cpp

all: server client

server: server.o
	$(COMPILER) $< -o $@

server.o: server.c
	$(COMPILER) $(flags) $<

client: client.o 
	$(COMPILER) $< -o $@ -lm

client.o: client.c  
	$(COMPILER) $(flags) $<

clean:
	rm -rf *o server client