#compiler
COMPILER=g++
flags=-c -Wall

#File Variable
OBJECTS=main.o 
main=main.cpp

q3: $(OBJECTS) 
	$(COMPILER) $< -lpthread -o $@

main.o: $(main)
	$(COMPILER) $(flags) $<

clean:
	rm -rf *o q3