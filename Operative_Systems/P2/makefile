all: clean executable valgrind
no-valgrind: clean executable execute

executable: list.o aux_functions.o functions.o p2.o
	gcc -Wall -o p2 list.o aux_functions.o functions.o p2.o

%.o: %.c
	gcc -Wall -Wno-stringop-overflow -c -g $<

execute:
	./p2

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./p2

clean:
	rm -f *.o *.txt p2
	
ref:
	./shell.out

ref-valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./shell.out
