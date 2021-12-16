SRCMODULES = task1.c task2.c task3.c task4.c
OBJMODULES = $(SRCMODULES:.c=.o)
CFLAGS = -g -Wall
CC = gcc

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

shell1: main1.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

shell2: main2.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

shell3: main3.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

shell4: main4.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

msh: main5.c $(OBJMODULES) task5.o
	$(CC) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean: 
	rm -f *.o shell*
