all: execute

executable:
	gcc -Wall p2.c -lm -o p2

execute: clean executable
	./p2

valgrind: clean executable
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-report.txt ./p2

clean:
	rm -f p2
