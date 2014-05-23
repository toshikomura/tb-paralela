
grade=$1
iteracoes=$2

for cores in 1 2 4 6
	do
	for num in $(seq ${iteracoes})
		do
		./rbgs ${grade} ${grade} ${cores} 2000 g > pass${grade}x${cores}.txt
		done  
	done
