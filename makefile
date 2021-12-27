all: huffmandriver.o huffmannode.o huffmantree.o
	g++ -o huffencode huffmandriver.o huffmannode.o huffmantree.o -std=c++11

test: huffmannodetests.cpp huffmantreetests.cpp huffmannode.cpp huffmannode.h huffmantree.cpp huffmantree.h
	g++ -o huffmantests huffmannodetests.cpp huffmantreetests.cpp huffmannode.cpp huffmantree.cpp -std=c++11
	./huffmantests

huffmandriver.o:
	g++ -c huffmandriver.cpp -std=c++11

huffmannode.o: huffmannode.cpp huffmannode.h
	g++ -c huffmannode.cpp -std=c++11

huffmantree.o: huffmantree.cpp huffmantree.h
	g++ -c huffmantree.cpp -std=c++11

clean:
	@rm -f *.o
	@rm -f *.bin
	@rm -f *.txt
	@rm -f huffencode
	@rm -f huffmantests
	@rm -f *.hdr
