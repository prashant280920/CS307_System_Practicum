#compiler
COMPILER=g++
flags=-c

#File Variable
OBJECTS=main.o 
main=main.cpp

all : q3 script


q3: main.o
	$(COMPILER) $< -lpthread -o $@

main.o: main.cpp
	$(COMPILER) $(flags) $<

script: script.o 
	$(COMPILER) $< -o $@

script.o: script.cpp
	$(COMPILER) $(flags) $<

clean:
	rm -rf *o q3 script input.txt output.txt