TARGET = p3
CC = gcc
CFLAGS = -Wall -Wno-stringop-overflow
LIBS = -L/usr/lib/x86_64-linux-gnu -lreadline -lcurses -I/usr/include/readline

SRCS = list.c aux_functions.c functions.c p3.c
OBJS = $(SRCS:.c=.o)

.PHONY: all no-valgrind executable execute valgrind clean

all: clean executable valgrind

no-valgrind: clean executable execute

executable: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -g $<

execute:
	./$(TARGET)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(TARGET)

clean:
	rm -f *.o *.txt $(TARGET)
