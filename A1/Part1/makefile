#compiler
COMPILER=g++
flags=-c -Wall 

#File Variable
OBJECTS=shell.o 
main=shell.cpp


myshell: $(OBJECTS)
	$(COMPILER) $< -o $@

shell.o: $(main)
	$(COMPILER) $(flags) $<

clean:
	rm -rf *o myshell