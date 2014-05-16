all: rbgs
rbgs: io.o matematica.o definicoes.h rbgs.c
	gcc rbgs.c matematica.o io.o -o rbgs -lm -O3 -Wall -Winline -Wshadow -fopenmp

io.o: io.c matematica.o definicoes.h
	gcc -c io.c -o io.o -lm -O3 -Wall -Winline -Wshadow -fopenmp

matematica.o: matematica.c definicoes.h
	gcc -c matematica.c -o matematica.o -lm -O3 -Wall -Winline -Wshadow -fopenmp

exec:
	./rbgs 10000 10000 2 100 j
	./rbgs 10000 10000 2 100 g
	./rbgs 10000 10000 4 100 j
	./rbgs 10000 10000 4 100 g

clean:
	rm -f rbgs
	rm -rf *.o
	rm -f *.png
	rm -f solution.txt
