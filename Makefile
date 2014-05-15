all:
	gcc rbgs.c -o rbgs -lm -O3 -Wall -Winline -Wshadow -fopenmp

exec:
	./rbgs 10000 10000 2 100 j
	./rbgs 10000 10000 2 100 g
	./rbgs 10000 10000 4 100 j
	./rbgs 10000 10000 4 100 g

clean:
	rm -rf rbgs
	rm -rf *.o
	rm -rf *.png
	rm solution.txt
