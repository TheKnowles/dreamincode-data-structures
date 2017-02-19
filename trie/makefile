all: clean Trie 

Trie: main.o 
	g++ main.o -o Trie

main.o: src/main.cpp
	g++ -c -std=c++11 src/main.cpp -I.

.PHONY:
clean: 
	rm -f *.o
	rm -f Trie
