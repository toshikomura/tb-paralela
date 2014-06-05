#!/bin/bash

# função para testar se os parametros estão corretos
function usage {
	echo uso:
	echo ${0} '"{repetições}" {threadini} {threadfim}'

	exit 1
}

# Testa se foi passado o número correto de paramentos
if [ ! ${#} -eq 3 ] ;	then
	usage
fi

iteracoes=$1
threadini=$2
threadfim=$3

for grade in 32 33 1024 1025 2048 2049 ; do
    for cores in 8 4 2 1 ; do
	    for num in $(seq ${iteracoes}) ; do
		    likwid-pin -q -c ${threadini}-${threadfim} ./rgbs ${grade} ${grade} ${cores} 2000 g >> pass${grade}x${cores}.txt
	    done
	    echo "${grade} ${cores}"
        echo "${grade} ${cores}" >> testes/medias${grade}x${cores}.txt
	    ./media pass${grade}x${cores}.txt
	    ./media pass${grade}x${cores}.txt >> testes/medias${grade}x${cores}.txt
	    rm -f pass${grade}x${cores}.txt
    done
done
