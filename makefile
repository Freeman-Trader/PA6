PA5: main.cpp
	g++ -g -Wall -std=c++11 -fopenmp -o PA5 main.cpp
	
Hash.o: Hash.hpp
	g++ -g -Wall -std=c++11 -o Hash.hpp
	
ChainingHash.o: ChainingHash.hpp
	g++ -g -Wall -std=c++11 -o ChainingHash.hpp
	
ProbingHash.o: ProbingHash.hpp
	g++ -g -Wall -std=c++11 -o ProbingHash.hpp
	
clean:
	-rm PA5
	
run:
	clear
	./PA5