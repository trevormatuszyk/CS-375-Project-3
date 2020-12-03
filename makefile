CFLAGS = -Wall -Wextra -DDEBUG -g -pedantic -std=c++14

A = algorithms

$(A): $(A).o
	g++ $(CFLAGS) -o $(A) $(A).o

$(A).o: $(A).cpp
	g++ $(CFLAGS) -c $(A).cpp -o $(A).o


all: $(A)

run: all
	./$(A) badImprovedGreedyInput.txt outputbadImprovedGreedy2.txt 1




checkmem: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(A) input.txt outputFile.txt 1

clean:
	rm -f $(A).o
	rm -f $(A)
	rm -f submission
