all: rgbs media
rgbs: io.o matematica.o definicoes.h rgbs.c
	gcc rgbs.c matematica.o io.o -o rgbs -lm -O3 -Wall -Winline -Wshadow -fopenmp


io.o: io.c matematica.o definicoes.h
	gcc -c io.c -o io.o -lm -O3 -Wall -Winline -Wshadow -fopenmp

matematica.o: matematica.c definicoes.h
	gcc -c matematica.c -o matematica.o -lm -O3 -Wall -Winline -Wshadow -fopenmp

exec:
	./rgbs 2049 2049 8 5000 j

media: media.c
	gcc media.c -o media

exectest:
	./rgbs 32 32 1 2000 g > testes/resultado_32_1_2000.txt
	./rgbs 32 32 2 2000 g > testes/resultado_32_2_2000.txt
	./rgbs 32 32 4 2000 g > testes/resultado_32_4_2000.txt
	./rgbs 32 32 6 2000 g > testes/resultado_32_6_2000.txt
	./rgbs 33 33 1 2000 g > testes/resultado_33_1_2000.txt
	./rgbs 33 33 2 2000 g > testes/resultado_33_2_2000.txt
	./rgbs 33 33 4 2000 g > testes/resultado_33_4_2000.txt
	./rgbs 33 33 6 2000 g > testes/resultado_33_6_2000.txt
	./rgbs 1024 1024 1 2000 g > testes/resultado_1024_1_2000.txt
	./rgbs 1024 1024 2 2000 g > testes/resultado_1024_2_2000.txt
	./rgbs 1024 1024 4 2000 g > testes/resultado_1024_4_2000.txt
	./rgbs 1024 1024 6 2000 g > testes/resultado_1024_6_2000.txt
	./rgbs 1025 1025 1 2000 g > testes/resultado_1025_1_2000.txt
	./rgbs 1025 1025 2 2000 g > testes/resultado_1025_2_2000.txt
	./rgbs 1025 1025 4 2000 g > testes/resultado_1025_4_2000.txt
	./rgbs 1025 1025 6 2000 g > testes/resultado_1025_6_2000.txt
	./rgbs 2048 2048 1 2000 g > testes/resultado_2048_1_2000.txt
	./rgbs 2048 2048 2 2000 g > testes/resultado_2048_2_2000.txt
	./rgbs 2048 2048 4 2000 g > testes/resultado_2048_4_2000.txt
	./rgbs 2048 2048 6 2000 g > testes/resultado_2048_6_2000.txt
	./rgbs 2049 2049 1 2000 g > testes/resultado_2049_1_2000.txt
	./rgbs 2049 2049 2 2000 g > testes/resultado_2049_2_2000.txt
	./rgbs 2049 2049 4 2000 g > testes/resultado_2049_4_2000.txt
	./rgbs 2049 2049 6 2000 g > testes/resultado_2049_6_2000.txt

clean:
	rm -f rgbs media
	rm -rf *.o
	rm -f *.png
	rm -f solution.txt
