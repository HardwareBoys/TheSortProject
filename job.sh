#!/bin/bash

ALGOS=(radixsort mergesort inplace-mergesort);
TESTS=(ran dis rev asc rdi);
FROM=$((2**16));
TO=$((2**23));
echo "COMPILING ALGOS";
mkdir tmp;
g++ -std=c++11 -o ./tmp/generator ./generator.cpp;
for i in ${ALGOS[*]}
do
	g++ -std=c++11 -o ./tmp/$i ./$i.cpp;
done

echo "GENERATING TEST DATA";
for ((i=FROM;i<=TO;i*=2)) 
do 
	./tmp/generator $i      	       	        	> ./tmp/ran${i}.test;
	./tmp/generator $i 1000 	       	        	> ./tmp/dis${i}.test;
	./tmp/generator $i      | ./tmp/radixsort -v -r > ./tmp/rev${i}.test;
	./tmp/generator $i      | ./tmp/radixsort -v    > ./tmp/asc${i}.test;
	./tmp/generator $i 1000 | ./tmp/radixsort -v -r > ./tmp/rdi${i}.test;
done

echo "RUNNING TESTS";
for i in ${ALGOS[*]}
do
	for ((j=FROM;j<=TO;j*=2)) 
	do 
		for k in ${TESTS[*]}
		do
			TIME=`/usr/bin/time -f "%e" ./tmp/$i < ./tmp/$k${j}.test 2>&1`;
			echo -e "ALGO: $i TEST: $k$j\t TIME: $TIME";
			echo -e $j"\t"$TIME >> ./tmp/$i"_"$k.out;
		done
	done 
done

for i in ${TESTS[*]}
do
    gnuplot <<- EOF
	set terminal svg size 800,600 fixed enhanced
	set output '${i}.svg'	
	set origin 0.0,0.0
	set ylabel "time"
	set xlabel "N"
	set format x "%.0s*10^%T"
	set logscale x	
	set logscale y
	set key box left top
	set multiplot
	`echo -n "plot "
	for j in ${ALGOS[*]}
	do
		echo -n "'./tmp/${j}_${i}.out' using 1:2 title '$j' smooth unique with line axes x1y2, "
	done`	
	show key
	unset multiplot	
	EOF
done
rm -rf ./tmp;














