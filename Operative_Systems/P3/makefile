all: clean executable valgrind
no-valgrind: clean executable execute

executable: list.o aux_functions.o functions.o p3.o
	gcc -Wall -o p3 list.o aux_functions.o functions.o p3.o

%.o: %.c
	gcc -Wall -Wno-stringop-overflow -c -g $<

execute:
	./p3

valgrind:
	mkdir valgrind
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind/valgrind-report.txt ./p3

clean:
	rm -f *.o *.txt p3
	if [ -d "valgrind" ]; then rm -r valgrind; fi