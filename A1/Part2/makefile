#compiler
COMPILER=g++
flags=-c -Wall

#File Variable
OBJECTS=main.o 
main=main.cpp


main: $(OBJECTS)
	$(COMPILER) $(OBJECTS) -lpthread -o q2

main.o: $(main)
	$(COMPILER) $(flags) $(main)

clean:
	rm -rf *o q2