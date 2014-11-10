#!/bin/bash

#  ===========================================================================
# |                                                                           |
# |             COMMAND FILE FOR SUBMITTING SGE JOBS                          |
# |                                                                           |
# |                                                                           |
# | SGE keyword statements begin with #$                                      |
# |                                                                           |
# | Comments begin with #                                                     |
# | Any line whose first non-blank character is a pound sign (#)              |
# | and is not a SGE keyword statement is regarded as a comment.              |
#  ===========================================================================

# Request Bourne shell as shell for job
#$ -S /bin/bash

# Execute the job from the current working directory.
#$ -cwd

# Defines  or  redefines  the  path used for the standard error stream of the job.
#$ -e .

# The path used for the standard output stream of the job.
#$ -o .

# Do not change.
#$ -pe ompi 1

# Do not change.
#$ -q 12c_1slots_per_host.q

ALGOS=(radixsort10 radixsort8192 mergesort inplace-mergesort);
TESTS=(ran dis rev asc rdi);
FROM=$((2**14));
TO=$((2**20));

REC_FROM=2
REC_TO=100
REC_STEP=12

OPTIMALIZATIONS=(
"-O2 -mtune=generic" 
"-O3 -mtune=generic" 
"-Ofast -mtune=generic" 
"-O3 -msse4.2 -mfpmath=sse -mtune=generic" 
"-O3 -msse4.2 -mfpmath=sse -mtune=generic" 
"-O3 -msse4.2 -mfpmath=sse -mfancy-math-387 -mtune=native" 
"-O3 -msse4.2 -mfpmath=sse -mfancy-math-387 -march=native" 
"-O3 -msse4.2 -mfpmath=sse -mfancy-math-387 -march=native -mregparm=3 -msseregparm" 
"-O3 -msse4.2 -mfpmath=sse -mfancy-math-387 -march=native -mregparm=3 -msseregparm -fno-finite-math-only -fno-trapping-math -ffast-math -mrecip=all"
);


echo "COMPILING ALGOS";
mkdir -p tmp;
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
	./tmp/generator $i      | ./tmp/radixsort8192 -v -r > ./tmp/rev${i}.test;
	./tmp/generator $i      | ./tmp/radixsort8192 -v    > ./tmp/asc${i}.test;
	./tmp/generator $i 1000 | ./tmp/radixsort8192 -v -r > ./tmp/rdi${i}.test;
done

echo "COMPARING ALGOS";
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
	set terminal svg size 1400,600 fixed enhanced
	set output 'algos_${i}.svg'	
	set origin 0.0,0.0
	set ylabel "time"
	set xlabel "N"
	set xrange [*:]
	set format x "%.0s*10^%T"
	set logscale x	
##    set logscale y
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

## compare of rotations

for ((j=FROM;j<=TO;j*=2)) 
do 
		TIME=`/usr/bin/time -f "%e" ./tmp/inplace-mergesort worse < ./tmp/ran$j.test 2>&1`;
		echo -e "ALGO: inplace-mergesort (worse rotation) TEST: ran$j\t TIME: $TIME";
		echo -e $j"\t"$TIME >> ./tmp/"inplace-mergesort-worse_"ran.out;
done 

gnuplot <<- EOF
set terminal svg size 1400,600 fixed enhanced
set output 'rotations.svg'	
set origin 0.0,0.0
set ylabel "time"
set xlabel "N"
set xrange [*:]
set format x "%.0s*10^%T"
set logscale x	
##set logscale y
set key box left top
set multiplot
plot './tmp/inplace-mergesort_ran.out' using 1:2 title 'optimized rotations' smooth unique with line axes x1y2, './tmp/inplace-mergesort-worse_ran.out' using 1:2 title 'simple rotations' smooth unique with line axes x1y2	
show key
unset multiplot	
EOF

DEFAULT='-std=c++11 ./inplace-mergesort.cpp -o ./tmp/inplace-mergesort_opt';

##compare compile options

for ((i=0; i<9; i++))
do
	g++ ${OPTIMALIZATIONS[$i]} ${DEFAULT}${i};
done

for ((j=FROM;j<=TO;j*=2)) 
do 
	for ((i=0; i<9; i++))
	do
		TIME=`/usr/bin/time -f "%e" ./tmp/inplace-mergesort_opt$i < ./tmp/ran$j.test 2>&1`;
		echo -e "OPTI: $i TEST: ran$j\t TIME: $TIME";
		echo -e $j"\t"$TIME >> ./tmp/"inplace-mergesort_opt"$i.out;
	done
done

gnuplot <<- EOF
set terminal svg size 1400,600 fixed enhanced
set output 'optimalizations.svg'	
set origin 0.0,0.0
set ylabel "time"
set xlabel "N"
set xrange [*:]
set format x "%.0s*10^%T"
set logscale x	
##set logscale y
set key box left bottom
set multiplot
`echo -n "plot "
for ((i=0; i<9; i++))
do
	echo -n "'./tmp/"inplace-mergesort_opt"$i.out' using 1:2 title '${OPTIMALIZATIONS[$i]}' smooth unique with line axes x1y2, "
done`	
show key
unset multiplot	
EOF


for ((j=FROM;j<=TO;j*=2)) 
do 
	for ((i=REC_FROM; i<REC_TO; i+=REC_STEP))
	do
		TIME=`/usr/bin/time -f "%e" ./tmp/inplace-mergesort dummy $i < ./tmp/ran$j.test 2>&1`;
		echo -e "REC: $i TEST: ran$j\t TIME: $TIME";
		echo -e $j"\t"$TIME >> ./tmp/"inplace-mergesort_rec"$i.out;
	done
done

gnuplot <<- EOF
set terminal svg size 1400,600 fixed enhanced
set output 'recursion.svg'	
set origin 0.0,0.0
set ylabel "time"
set xlabel "N"
set xrange [*:]
set format x "%.0s*10^%T"
set logscale x	
##set logscale y
set key box left bottom
set multiplot
`echo -n "plot "
for ((i=REC_FROM; i<REC_TO; i+=REC_STEP))
do
	echo -n "'./tmp/"inplace-mergesort_rec"$i.out' using 1:2 title 'recursion depth $i' smooth unique with line axes x1y2, "
done`	
show key
unset multiplot	
EOF

rm -rf ./tmp;














