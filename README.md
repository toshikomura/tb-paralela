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

Dpois o gnuplot aberto executar:

gnuplot> set terminal xterm
gnuplot> set output "solution.png"
gnuplot> splot "solution.txt"


Como ver o plano cartesiano:

Basta abrir o arquivo "solution.png"


Como Limpar:

Basta entrar no diretório onde se localiza o Makefile e executar:

$ make clean
