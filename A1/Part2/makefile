#compiler
COMPILER=g++
flags=-c -Wall


run: main1 main2 main3


main1: main1.o
	$(COMPILER) $< -lpthread -o $@

main2: main2.o
	$(COMPILER) $< -lpthread -o $@

main3: main3.o
	$(COMPILER) $< -lpthread -o $@

main1.o: main1.cpp
	$(COMPILER) $(flags)  $<

main2.o: main2.cpp
	$(COMPILER) $(flags)  $<

main3.o: main3.cpp
	$(COMPILER) $(flags)  $<

clean:
	rm -rf *o main1 main2 main3