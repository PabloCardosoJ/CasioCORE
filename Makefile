CC = gcc

project: main.o queue.o scheduler.o rtcc.o
	$(CC) main.o queue.o scheduler.o rtcc.o -o main -g

main.o: main.c queue.h scheduler.h rtcc.h
	$(CC) -c app/main.c -o main.o -g

queue.o: queue.c queue.h scheduler.h rtcc.h
	$(CC) -c app/queue.c -o queue.o -g

scheduler.o: scheduler.c queue.h scheduler.h rtcc.h
	$(CC) -c app/scheduler.c -o scheduler.o -g

rtcc.o: rtcc.c queue.h scheduler.h rtcc.h
	$(CC) -c app/rtcc.c -o rtcc.o -g


#---Generates project documentation with doxygen---------------------------------------------------
docs :
	doxygen .doxyfile
	chrome Build/doxygen/html/index.html