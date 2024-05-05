 all:
	gcc -Wall -IApp -c App/rtcc.c -o rtcc.o
	gcc -Wall -IApp -c App/queue.c -o queue.o
	gcc -Wall -IApp -c App/scheduler.c -o scheduler.o
	gcc -Wall -IApp -c App/main.c -o main.o
	gcc main.o rtcc.o queue.o scheduler.o -o main.exe
	./main.exe
	
clean:
	rm *.exe *.o 