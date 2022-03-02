#compiler
COMPILER=g++
flags=-c -Wall 

#File Variable
OBJECTS=shell.o 
main=shell.cpp


myshell: $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o myshell

shell.o: $(main)
	$(COMPILER) $(flags) $(main)

clean:
	rm -rf *o myshell