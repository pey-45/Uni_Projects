all: execute

executable:
	gcc -Wall p3.c -lm -o p3

execute: clean executable
	./p3

valgrind: clean executable
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-report.txt ./p3

clean:
	rm -f p3
