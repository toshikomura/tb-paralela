#!/bin/bash

# função para testar se os parametros estão corretos
function usage {
	echo uso:
	echo ${0} '{tamanho da grade} "{repetições}" {threadini} {threadfim}'

	exit 1
}

# Testa se foi passado o número correto de paramentos
if [ ! ${#} -eq 4 ] ;	then
	usage
fi

grade=$1
iteracoes=$2
threadini=$3
threadfim=$4

for cores in 1 2 4 8 ; do
	for num in $(seq ${iteracoes}) ; do
		likwid-pin -q -c ${threadini}-${threadfim} ./rbgs ${grade} ${grade} ${cores} 2000 g >> pass${grade}x${cores}.txt
	done
	echo ${cores}
	./media pass${grade}x${cores}.txt
	./media pass${grade}x${cores}.txt >> testes/medias${grade}x${cores}.txt
	rm -f pass${grade}x${cores}.txt
done
