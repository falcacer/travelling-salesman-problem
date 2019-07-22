programa: tsp.c slist.o
	gcc -g -o programa slist.o tsp.c -O
slist.o: slist.c slist.h
	gcc -g -c slist.c -Wall -pedantic -O
clean:
	rm -f programa *.o