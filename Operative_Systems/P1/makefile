all: clean executable valgrind
no-valgrind: clean executable execute

executable: list.o aux_functions.o functions.o p1.o
	gcc -Wall -o p1 list.o aux_functions.o functions.o p1.o

%.o: %.c
	gcc -Wall -Wno-stringop-overflow -c -g $<

execute:
	./p1

valgrind:
	mkdir valgrind
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind/valgrind-report.txt ./p1

clean:
	rm -f *.o *.txt p1
	if [ -d "valgrind" ]; then rm -r valgrind; fi
