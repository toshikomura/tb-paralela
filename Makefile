all:
	gcc rbgs.c -o rbgs -lm -O3 -Wall -Winline -Wshadow -fopenmp

exec:
	./rbgs 10 10 2 100 j
#	./rbgs 2 2 2 2 a

clean:
	rm -rf rbgs
	rm -rf *.o
	rm -rf *.png
