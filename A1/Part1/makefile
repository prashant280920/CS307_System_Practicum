#compiler
COMPILER=g++
flags=-c -Wall 

#File Variable
OBJECTS=shell.o 
main=shell.cpp

run: myshell test

myshell: $(OBJECTS)
	$(COMPILER) $< -o $@

shell.o: $(main)
	$(COMPILER) $(flags) $<

test: test.o
	$(COMPILER) $< -o $@

test.o: test.cpp
	$(COMPILER) $(flags) $<

clean:
	rm -rf *o myshell test history.txt