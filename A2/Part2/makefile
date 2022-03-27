#compiler
COMPILER=g++
flags=-c

#File Variable
OBJECTS=main.o 
main=main.cpp


q2: $(OBJECTS) 
	$(COMPILER) $< -lpthread -o $@

main.o: $(main)
	$(COMPILER) $(flags) $<

clean:
	rm -rf *o q2