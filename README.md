tb-paralela
===========

Trabalho de paralela - Equação Diferencial Parcial

Atenção: Este trabalho pode ser consultado para o entendimento
do problema e de uma possivel solução que pode ser melhorada.
A entrega de uma cópia legitima deste código não é de
responsabilidade dos autores.

Como Compilar:

Basta entrar no diretório onde se localiza o Makefile e executar:

$ make

Como Executar:

Basta entrar no diretório onde se localiza o Makefile e executar:

$ make exec


Arquivo Gerado na execução:

"solution.txt" que serve para plotar o plano cartesiano com o gnuplot.


Como plotar o plano cartesiano com o gnuplot:

Com o programa gnuplot instalado executar:

$ gnuplot

Depois com o gnuplot aberto executar:

gnuplot> set term png
Terminal type set to 'png'
Options are 'nocrop font "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf,12" fontscale 1.0 size 640,480 '
gnuplot> set dgrid3d 30,30
gnuplot> set output "solution.png"
gnuplot> splot "solution.txt" u 1:2:3 with lines


Como ver o plano cartesiano:

Basta abrir o arquivo "solution.png"


Como Limpar:

Basta entrar no diretório onde se localiza o Makefile e executar:

$ make clean


Consultar Threads da Máquina

Executar:

$ likwid-pin -p


Pinar Threads

Executar:

$ likwid-pin -c N:31,32,33,34  ./rbgs 2049 2049 4 2000 g
ou
$ likwid-pin -c N:31-34  ./rbgs 2049 2049 4 2000 g
ou
$ likwid-pin -c 31-34  ./rbgs 2049 2049 4 2000 g


Como utilizar o comando shell "screen"

Executar:

$ screen

Dar um enter para abrir o novo terminal
Executar comandos desejados
Para sair e deixar os comandos executando

Apertar:

Ctrl + a + d

Para verificar quais terminais estão rodando

Executar:

$ screen -r

Para entrar no terminal desejado

Executar

$ screen -r (número da tela)
