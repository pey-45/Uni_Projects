all: execute

executable:
	gcc -Wall p1.c -lm -o p1

execute: clean executable
	./p1

valgrind: clean executable
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-report.txt ./p1

clean:
	rm -f p1
